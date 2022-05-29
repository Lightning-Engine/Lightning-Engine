#include "li/view/impl.h"

#include "li/std.h"

const struct li_view_impl *li_view_impl;

li_view_key_fun_t    li_view_key_fun    = NULL;
li_view_motion_fun_t li_view_motion_fun = NULL;

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

/* TODO: generalize */
void li_view_poll(void) {
    li_view_impl->poll();
}

void li_view_send_key(li_view_msg_t msg) {
    if (li_view_key_fun) {
        li_view_key_fun(msg);
    }
}

void li_view_send_motion(li_view_msg_t msg, int x, int y, int id) {
    if (li_view_motion_fun) {
        li_view_motion_fun(msg, x, y, id);
    }
}
