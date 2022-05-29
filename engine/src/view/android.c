#include "li/view/android.h"

#include "li/main/android.h"

const struct li_view_impl li_view_android_impl = { li_view_android_init,
                                                   li_view_android_poll };

int li_view_android_init(void) {
    li_view_impl                     = &li_view_android_impl;
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

int32_t li_view_android_event(struct android_app *app, AInputEvent *event) {
    (void) app;
    switch (AInputEvent_getType(event)) {
    case AINPUT_EVENT_TYPE_MOTION:
        li_view_send_motion();
        return 1;
    case AINPUT_EVENT_TYPE_KEY:
        li_view_send_key();
        return 1;
    }
    return 0;
}
