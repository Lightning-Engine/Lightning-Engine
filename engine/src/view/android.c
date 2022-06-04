#include "li/view/android.h"

#include "li/main/android.h"

#include <android/log.h>
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
/* TODO: or maybe implement in java */
void li_view_android_show_kb(int show) {
    JNIEnv *env;
    JavaVM *vm       = li_main_android_app->activity->vm;
    jobject activity = li_main_android_app->activity->clazz;
    jobject input_method_service, input_method_manager, window, decor_view,
        binder;
    jclass    NativeActivity, Context, InputMethodManager, Window, View;
    jmethodID getSystemService, getWindow, getDecorView, getWindowToken,
        showSoftInput, hideSoftInputFromWindow;
    jfieldID INPUT_METHOD_SERVICE;
    (*vm)->AttachCurrentThread(vm, &env, NULL);
    NativeActivity = (*env)->FindClass(env, "android/app/NativeActivity");
    Context        = (*env)->FindClass(env, "android/content/Context");
    InputMethodManager =
        (*env)->FindClass(env, "android/view/inputmethod/InputMethodManager");
    Window           = (*env)->FindClass(env, "android/view/Window");
    View             = (*env)->FindClass(env, "android/view/View");
    getSystemService = (*env)->GetMethodID(
        env, NativeActivity, "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;");
    getWindow = (*env)->GetMethodID(
        env, NativeActivity, "getWindow", "()Landroid/view/Window;");
    getDecorView = (*env)->GetMethodID(
        env, Window, "getDecorView", "()Landroid/view/View;");
    getWindowToken = (*env)->GetMethodID(
        env, View, "getWindowToken", "()Landroid/os/IBinder;");
    showSoftInput = (*env)->GetMethodID(
        env, InputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z");
    hideSoftInputFromWindow = (*env)->GetMethodID(
        env, InputMethodManager, "hideSoftInputFromWindow",
        "(Landroid/os/IBinder;I)Z");
    INPUT_METHOD_SERVICE = (*env)->GetStaticFieldID(
        env, Context, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    input_method_service =
        (*env)->GetStaticObjectField(env, Context, INPUT_METHOD_SERVICE);
    input_method_manager = (*env)->CallObjectMethod(
        env, activity, getSystemService, input_method_service);
    window     = (*env)->CallObjectMethod(env, activity, getWindow);
    decor_view = (*env)->CallObjectMethod(env, window, getDecorView);
    if (show) {
        (*env)->CallBooleanMethod(
            env, input_method_manager, showSoftInput, decor_view, 0);
    } else {
        binder = (*env)->CallObjectMethod(env, decor_view, getWindowToken);
        (*env)->CallBooleanMethod(
            env, input_method_manager, hideSoftInputFromWindow, binder, 0);
    }
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
        /* TODO: better repeat handling */
        li_view_send_key(li_view_msg_key_up, state, key);
        break;
    case AKEY_EVENT_ACTION_MULTIPLE:
        /* TODO: character input (also other platforms) */
        break;
    }
}

void li_view_android_event_touch(AInputEvent *event) {
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
            li_view_send_touch(li_view_msg_touch_down, state, x, y, id);
            break;
        case AMOTION_EVENT_ACTION_POINTER_UP:
            if (id != action >> 8) {
                break;
            }
        case AMOTION_EVENT_ACTION_UP:
            li_view_send_touch(li_view_msg_touch_up, state, x, y, id);
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            li_view_send_touch(li_view_msg_touch_move, state, x, y, id);
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
        li_view_android_event_touch(event);
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

/* TODO: device specific keys */
li_input_key_t li_view_android_get_key(int key) {
    switch (key) {
    case AKEYCODE_0:
        return LI_INPUT_KEY_0;
    case AKEYCODE_1:
        return LI_INPUT_KEY_1;
    case AKEYCODE_2:
        return LI_INPUT_KEY_2;
    case AKEYCODE_3:
        return LI_INPUT_KEY_3;
    case AKEYCODE_4:
        return LI_INPUT_KEY_4;
    case AKEYCODE_5:
        return LI_INPUT_KEY_5;
    case AKEYCODE_6:
        return LI_INPUT_KEY_6;
    case AKEYCODE_7:
        return LI_INPUT_KEY_7;
    case AKEYCODE_8:
        return LI_INPUT_KEY_8;
    case AKEYCODE_9:
        return LI_INPUT_KEY_9;
    case AKEYCODE_A:
        return LI_INPUT_KEY_A;
    case AKEYCODE_B:
        return LI_INPUT_KEY_B;
    case AKEYCODE_C:
        return LI_INPUT_KEY_C;
    case AKEYCODE_D:
        return LI_INPUT_KEY_D;
    case AKEYCODE_E:
        return LI_INPUT_KEY_E;
    case AKEYCODE_F:
        return LI_INPUT_KEY_F;
    case AKEYCODE_G:
        return LI_INPUT_KEY_G;
    case AKEYCODE_H:
        return LI_INPUT_KEY_H;
    case AKEYCODE_I:
        return LI_INPUT_KEY_I;
    case AKEYCODE_J:
        return LI_INPUT_KEY_J;
    case AKEYCODE_K:
        return LI_INPUT_KEY_K;
    case AKEYCODE_L:
        return LI_INPUT_KEY_L;
    case AKEYCODE_M:
        return LI_INPUT_KEY_M;
    case AKEYCODE_N:
        return LI_INPUT_KEY_N;
    case AKEYCODE_O:
        return LI_INPUT_KEY_O;
    case AKEYCODE_P:
        return LI_INPUT_KEY_P;
    case AKEYCODE_Q:
        return LI_INPUT_KEY_Q;
    case AKEYCODE_R:
        return LI_INPUT_KEY_R;
    case AKEYCODE_S:
        return LI_INPUT_KEY_S;
    case AKEYCODE_T:
        return LI_INPUT_KEY_T;
    case AKEYCODE_U:
        return LI_INPUT_KEY_U;
    case AKEYCODE_V:
        return LI_INPUT_KEY_V;
    case AKEYCODE_W:
        return LI_INPUT_KEY_W;
    case AKEYCODE_X:
        return LI_INPUT_KEY_X;
    case AKEYCODE_Y:
        return LI_INPUT_KEY_Y;
    case AKEYCODE_Z:
        return LI_INPUT_KEY_Z;
    case AKEYCODE_NUMPAD_0:
        return LI_INPUT_KEY_NUM0;
    case AKEYCODE_NUMPAD_1:
        return LI_INPUT_KEY_NUM1;
    case AKEYCODE_NUMPAD_2:
        return LI_INPUT_KEY_NUM2;
    case AKEYCODE_NUMPAD_3:
        return LI_INPUT_KEY_NUM3;
    case AKEYCODE_NUMPAD_4:
        return LI_INPUT_KEY_NUM4;
    case AKEYCODE_NUMPAD_5:
        return LI_INPUT_KEY_NUM5;
    case AKEYCODE_NUMPAD_6:
        return LI_INPUT_KEY_NUM6;
    case AKEYCODE_NUMPAD_7:
        return LI_INPUT_KEY_NUM7;
    case AKEYCODE_NUMPAD_8:
        return LI_INPUT_KEY_NUM8;
    case AKEYCODE_NUMPAD_9:
        return LI_INPUT_KEY_NUM9;
    case AKEYCODE_NUMPAD_DOT:
        return LI_INPUT_KEY_NUMDOT;
    case AKEYCODE_NUMPAD_ADD:
        return LI_INPUT_KEY_NUMPLUS;
    case AKEYCODE_NUMPAD_SUBTRACT:
        return LI_INPUT_KEY_NUMMINUS;
    case AKEYCODE_NUMPAD_MULTIPLY:
        return LI_INPUT_KEY_NUMSTAR;
    case AKEYCODE_NUMPAD_DIVIDE:
        return LI_INPUT_KEY_NUMSLASH;
    case AKEYCODE_NUMPAD_ENTER:
        return LI_INPUT_KEY_NUMENTER;
    case AKEYCODE_NUM_LOCK:
        return LI_INPUT_KEY_NUMLOCK;
    case AKEYCODE_F1:
        return LI_INPUT_KEY_F1;
    case AKEYCODE_F2:
        return LI_INPUT_KEY_F2;
    case AKEYCODE_F3:
        return LI_INPUT_KEY_F3;
    case AKEYCODE_F4:
        return LI_INPUT_KEY_F4;
    case AKEYCODE_F5:
        return LI_INPUT_KEY_F5;
    case AKEYCODE_F6:
        return LI_INPUT_KEY_F6;
    case AKEYCODE_F7:
        return LI_INPUT_KEY_F7;
    case AKEYCODE_F8:
        return LI_INPUT_KEY_F8;
    case AKEYCODE_F9:
        return LI_INPUT_KEY_F9;
    case AKEYCODE_F10:
        return LI_INPUT_KEY_F10;
    case AKEYCODE_F11:
        return LI_INPUT_KEY_F11;
    case AKEYCODE_F12:
        return LI_INPUT_KEY_F12;
    case AKEYCODE_SYSRQ:
        return LI_INPUT_KEY_PRINTSCR;
    case AKEYCODE_SCROLL_LOCK:
        return LI_INPUT_KEY_SCRLOCK;
    case AKEYCODE_BREAK:
        return LI_INPUT_KEY_PAUSE;
    case AKEYCODE_INSERT:
        return LI_INPUT_KEY_INSERT;
    case AKEYCODE_MOVE_HOME:
        return LI_INPUT_KEY_HOME;
    case AKEYCODE_PAGE_UP:
        return LI_INPUT_KEY_PAGEUP;
    case AKEYCODE_FORWARD_DEL:
        return LI_INPUT_KEY_DELETE;
    case AKEYCODE_MOVE_END:
        return LI_INPUT_KEY_END;
    case AKEYCODE_PAGE_DOWN:
        return LI_INPUT_KEY_PAGEDOWN;
    case AKEYCODE_SYSTEM_NAVIGATION_UP:
        return LI_INPUT_KEY_UP;
    case AKEYCODE_SYSTEM_NAVIGATION_DOWN:
        return LI_INPUT_KEY_DOWN;
    case AKEYCODE_SYSTEM_NAVIGATION_LEFT:
        return LI_INPUT_KEY_LEFT;
    case AKEYCODE_SYSTEM_NAVIGATION_RIGHT:
        return LI_INPUT_KEY_RIGHT;
    case AKEYCODE_ESCAPE:
        return LI_INPUT_KEY_ESCAPE;
    case AKEYCODE_DEL:
        return LI_INPUT_KEY_BSPACE;
    case AKEYCODE_TAB:
        return LI_INPUT_KEY_TAB;
    case AKEYCODE_SHIFT_LEFT:
        return LI_INPUT_KEY_LSHIFT;
    case AKEYCODE_SHIFT_RIGHT:
        return LI_INPUT_KEY_RSHIFT;
    case AKEYCODE_CTRL_LEFT:
        return LI_INPUT_KEY_LCONTROL;
    case AKEYCODE_CTRL_RIGHT:
        return LI_INPUT_KEY_RCONTROL;
    case AKEYCODE_META_LEFT:
        return LI_INPUT_KEY_LSUPER;
    case AKEYCODE_META_RIGHT:
        return LI_INPUT_KEY_RSUPER;
    case AKEYCODE_ALT_LEFT:
        return LI_INPUT_KEY_LALT;
    case AKEYCODE_ALT_RIGHT:
        return LI_INPUT_KEY_RALT;
    case AKEYCODE_CAPS_LOCK:
        return LI_INPUT_KEY_CAPSLOCK;
    case AKEYCODE_ENTER:
        return LI_INPUT_KEY_ENTER;
    case AKEYCODE_SPACE:
        return LI_INPUT_KEY_SPACE;
    case AKEYCODE_PERIOD:
        return LI_INPUT_KEY_DOT;
    case AKEYCODE_COMMA:
        return LI_INPUT_KEY_COMMA;
    case AKEYCODE_SEMICOLON:
        return LI_INPUT_KEY_COLON;
    case AKEYCODE_APOSTROPHE:
        return LI_INPUT_KEY_QUOTE;
    case AKEYCODE_LEFT_BRACKET:
        return LI_INPUT_KEY_LBRACKET;
    case AKEYCODE_RIGHT_BRACKET:
        return LI_INPUT_KEY_RBRACKET;
    case AKEYCODE_BACKSLASH:
        return LI_INPUT_KEY_BSLASH;
    case AKEYCODE_MINUS:
        return LI_INPUT_KEY_MINUS;
    case AKEYCODE_SLASH:
        return LI_INPUT_KEY_SLASH;
    case AKEYCODE_GRAVE:
        return LI_INPUT_KEY_TILDE;
    case AKEYCODE_EQUALS:
        return LI_INPUT_KEY_EQUAL;
    }
    return LI_INPUT_KEY_NULL;
}
