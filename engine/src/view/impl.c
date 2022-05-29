#include "li/view/impl.h"

#include "li/std.h"

li_view_key_fun_t li_view_key_fun = NULL;
li_view_motion_fun_t li_view_motion_fun = NULL;

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
