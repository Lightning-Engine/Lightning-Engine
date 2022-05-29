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
    li_view_android_impl.poll();
}

void li_view_send_key(void) {
    if (li_view_key_fun) {
        li_view_key_fun(li_view_key_msg);
    }
}

void li_view_send_motion(void) {
    if (li_view_motion_fun) {
        li_view_motion_fun(li_view_motion_msg);
    }
}
