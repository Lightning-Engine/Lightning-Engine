#ifndef LI_VIEW_IMPL_H
#define LI_VIEW_IMPL_H

#include "li/view/view.h"

struct li_view_impl {
    void (*poll)(void);
};

extern const struct li_view_impl li_view_android_impl;
extern const struct li_view_impl li_view_ios_impl;

void li_view_send_key(void);
void li_view_send_motion(void);

#endif