#include "li/view/impl.h"

#include "li/std.h"

const struct li_view_impl *li_view_impl;

li_view_key_fun_t   li_view_key_fun   = NULL;
li_view_touch_fun_t li_view_touch_fun = NULL;

int li_view_init(void) {
    int result = -1;
#if LI_VIEW_ANDROID
    if (result == -1) {
        result = li_view_android_impl.init();
    }
#endif
#if LI_VIEW_IOS
    if (result == -1) {
        result = li_view_ios_impl.init();
    }
#endif
    return result;
}

void li_view_poll(void) {
    li_view_impl->poll();
}

void li_view_show_kb(int show) {
    li_view_impl->show_kb(show);
}

void li_view_send_key(
    li_view_msg_t msg, li_input_state_t state, li_input_key_t key) {
    if (li_view_key_fun) {
        li_view_key_fun(msg, state, key);
    }
}

void li_view_send_touch(
    li_view_msg_t msg, li_input_state_t state, int x, int y, int id) {
    if (li_view_touch_fun) {
        li_view_touch_fun(msg, state, x, y, id);
    }
}
