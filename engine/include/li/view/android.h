#ifndef LI_VIEW_ANDROID_H
#define LI_VIEW_ANDROID_H

#include "li/view/impl.h"

#include <android_native_app_glue.h>

int  li_view_android_init(void);
void li_view_android_poll(void);

int32_t li_view_android_event(struct android_app *app, AInputEvent *event);

#endif
