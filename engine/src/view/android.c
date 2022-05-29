#include "li/view/android.h"

#include "li/main/android.h"

const struct li_view_impl li_view_android_impl = { li_view_android_init,
                                                   li_view_android_poll };

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

void li_view_android_event_key(AInputEvent *event) {
    li_view_send_key(li_view_msg_key);
}

void li_view_android_event_motion(AInputEvent *event) {
    int    action;
    size_t count;
    size_t i;
    int    x;
    int    y;
    int    id;
    action = AMotionEvent_getAction(event);
    count  = AMotionEvent_getPointerCount(event);
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
            li_view_send_motion(li_view_msg_motion_down, x, y, id);
            break;
        case AMOTION_EVENT_ACTION_POINTER_UP:
            if (id != action >> 8) {
                break;
            }
        case AMOTION_EVENT_ACTION_UP:
            li_view_send_motion(li_view_msg_motion_up, x, y, id);
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            li_view_send_motion(li_view_msg_motion, x, y, id);
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
