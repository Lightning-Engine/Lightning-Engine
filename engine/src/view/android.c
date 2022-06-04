#include "li/view/android.h"

#include "li/main/android.h"

#include <jni.h>

const struct li_view_impl li_view_android_impl = { li_view_android_init,
                                                   li_view_android_poll,
                                                   li_view_android_show_kb };

int li_view_android_init(void) {
    li_view_impl                      = &li_view_android_impl;
    li_main_android_app->onInputEvent = li_view_android_event;
    return 0;
}

void li_view_android_poll(void) {
    int                         events;
    void                       *data;
    struct android_poll_source *source;
    while (ALooper_pollAll(0, NULL, &events, &data) >= 0) {
        source = data;
        if (source != NULL) {
            source->process(li_main_android_app, source);
        }
    }
}

/* TODO: cache or prefetch methods */
void li_view_android_show_kb(int show) {
    JavaVM *vm = li_main_android_app->activity->vm;
    JNIEnv *env;
    jclass  NativeActivity;
    jobject activity;
    (*vm)->AttachCurrentThread(vm, &env, NULL);
    NativeActivity = (*env)->FindClass(env, "android/app/NativeActivity");
    (*vm)->DetachCurrentThread(vm);
}

void li_view_android_event_key(AInputEvent *event) {
    li_input_key_t   key;
    li_input_state_t state;
    state = li_view_android_get_state(AKeyEvent_getMetaState(event));
    key   = li_view_android_get_key(AKeyEvent_getKeyCode(event));
    switch (AKeyEvent_getAction(event)) {
    case AKEY_EVENT_ACTION_DOWN:
        if (AKeyEvent_getRepeatCount(event) == 0) {
            li_view_send_key(li_view_msg_key_down, state, key);
        } else {
            li_view_send_key(li_view_msg_key_repeat, state, key);
        }
        break;
    case AKEY_EVENT_ACTION_UP:
        li_view_send_key(li_view_msg_key_up, state, key);
        break;
    case AKEY_EVENT_ACTION_MULTIPLE:
        /* TODO */
        break;
    }
}

void li_view_android_event_motion(AInputEvent *event) {
    int              action;
    size_t           count;
    li_input_state_t state;
    size_t           i;
    int              x;
    int              y;
    int              id;
    action = AMotionEvent_getAction(event);
    count  = AMotionEvent_getPointerCount(event);
    state  = li_view_android_get_state(AMotionEvent_getMetaState(event));
    for (i = 0; i < count; i++) {
        x  = AMotionEvent_getX(event, i);
        y  = AMotionEvent_getY(event, i);
        id = AMotionEvent_getPointerId(event, i);
        switch (action & AMOTION_EVENT_ACTION_MASK) {
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
            if (id != action >> 8) {
                break;
            }
        case AMOTION_EVENT_ACTION_DOWN:
            li_view_send_motion(li_view_msg_motion_down, state, x, y, id);
            break;
        case AMOTION_EVENT_ACTION_POINTER_UP:
            if (id != action >> 8) {
                break;
            }
        case AMOTION_EVENT_ACTION_UP:
            li_view_send_motion(li_view_msg_motion_up, state, x, y, id);
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            li_view_send_motion(li_view_msg_motion, state, x, y, id);
            break;
        }
    }
}

int32_t li_view_android_event(struct android_app *app, AInputEvent *event) {
    (void) app;
    switch (AInputEvent_getType(event)) {
    case AINPUT_EVENT_TYPE_KEY:
        li_view_android_event_key(event);
        return 1;
    case AINPUT_EVENT_TYPE_MOTION:
        li_view_android_event_motion(event);
        return 1;
    }
    return 0;
}

li_input_state_t li_view_android_get_state(int state) {
    li_input_state_t result = 0;
    if (state & AMETA_SHIFT_ON) {
        result |= LI_INPUT_STATE_SHIFT;
    }
    if (state & AMETA_CAPS_LOCK_ON) {
        result |= LI_INPUT_STATE_CAPSLOCK;
    }
    if (state & AMETA_CTRL_ON) {
        result |= LI_INPUT_STATE_CONTROL;
    }
    if (state & AMETA_ALT_ON) {
        result |= LI_INPUT_STATE_ALT;
    }
    if (state & AMETA_NUM_LOCK_ON) {
        result |= LI_INPUT_STATE_NUMLOCK;
    }
    if (state & AMETA_META_ON) {
        result |= LI_INPUT_STATE_SUPER;
    }
    return result;
}

li_input_key_t li_view_android_get_key(int key) {
    return key;
}
