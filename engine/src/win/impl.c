#include "li/win/impl.h"

#include "li/std.h"
#include "li/win/win.h"

const struct li_win_impl *li_win_impl = NULL;

li_key_fun_t    li_win_key_fun    = NULL;
li_button_fun_t li_win_button_fun = NULL;
li_motion_fun_t li_win_motion_fun = NULL;
li_size_fun_t   li_win_size_fun   = NULL;
li_close_fun_t  li_win_close_fun  = NULL;

int li_win_init(void) {
    int result = -1;
#if LI_WIN_XLIB
    if (result == -1) {
        result = li_win_xlib_impl.init();
    }
#endif
#if LI_WIN_WIN32
    if (result == -1) {
        result = li_win_win32_impl.init();
    }
#endif
#if LI_WIN_COCOA
    if (result == -1) {
        result = li_win_cocoa_impl.init();
    }
#endif
    return result;
}

void li_win_exit(void) {
    li_win_impl->exit();
}

void li_win_poll(void) {
    li_win_impl->poll();
}

li_win_t li_win_create(int width, int height) {
    return li_win_impl->create(width, height);
}

void li_win_destroy(li_win_t win) {
    li_win_impl->destroy(win);
}

void li_win_send_key(
    li_win_t win, li_win_msg_t msg, li_input_state_t state,
    li_input_key_t key) {
    if (li_win_key_fun != NULL) {
        li_win_key_fun(win, msg, state, key);
    }
}

void li_win_send_button(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y,
    li_input_button_t button) {
    if (li_win_button_fun != NULL) {
        li_win_button_fun(win, msg, state, x, y, button);
    }
}

void li_win_send_motion(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y) {
    if (li_win_motion_fun != NULL) {
        li_win_motion_fun(win, msg, state, x, y);
    }
}

void li_win_send_size(li_win_t win, li_win_msg_t msg, int width, int height) {
    if (li_win_size_fun != NULL) {
        li_win_size_fun(win, msg, width, height);
    }
}

void li_win_send_close(li_win_t win, li_win_msg_t msg) {
    if (li_win_close_fun != NULL) {
        li_win_close_fun(win, msg);
    }
}
