#ifndef LI_VIEW_IMPL_H
#define LI_VIEW_IMPL_H

#include "li/view/view.h"

struct li_view_impl {
    int (*init)(void);
    void (*poll)(void);
    void (*show_kb)(int show);
};

extern const struct li_view_impl  li_view_android_impl;
extern const struct li_view_impl  li_view_ios_impl;
extern const struct li_view_impl *li_view_impl;

void li_view_send_key(
    li_view_msg_t msg, li_input_state_t state, li_input_key_t key);
void li_view_send_touch(
    li_view_msg_t msg, li_input_state_t state, int x, int y, int id);

#endif
