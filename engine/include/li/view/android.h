#ifndef LI_VIEW_ANDROID_H
#define LI_VIEW_ANDROID_H

#include "li/view/impl.h"

#include <android_native_app_glue.h>

int  li_view_android_init(void);
void li_view_android_poll(void);
void li_view_android_show_kb(int show);

void    li_view_android_event_key(AInputEvent *event);
void    li_view_android_event_motion(AInputEvent *event);
int32_t li_view_android_event(struct android_app *app, AInputEvent *event);

li_input_state_t li_view_android_get_state(int state);
li_input_key_t   li_view_android_get_key(int key);

#endif
