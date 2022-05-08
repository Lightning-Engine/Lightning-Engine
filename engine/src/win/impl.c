#include "li/win/impl.h"

#include "li/std.h"
#include "li/win/win.h"

#if LI_WIN_XLIB
# include "li/win/xlib.h"
#endif
#if LI_WIN_WIN32
# include "li/win/win32.h"
#endif
#if LI_WIN_COCOA
# include "li/win/cocoa.h"
#endif

const struct li_win_impl *li_win_impl = NULL;

li_key_fun_t    li_win_key_fun;
li_mouse_fun_t  li_win_mouse_fun;
li_resize_fun_t li_win_resize_fun;
li_close_fun_t  li_win_close_fun;

int li_win_init(void) {
    int result = -1;
#if LI_WIN_XLIB
    if (result == -1) {
        result = li_win_xlib_init();
    }
#endif
#if LI_WIN_WIN32
    if (result == -1) {
        result = li_win_win32_init();
    }
#endif
#if LI_WIN_COCOA
    if (result == -1) {
        result = li_win_cocoa_init();
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
    li_win_t win, li_win_msg_t action, li_key_code_t key,
    li_key_state_t state) {
    if (li_win_key_fun != NULL) {
        li_win_key_fun(win, action, key, state);
    }
}

void li_win_send_mouse(
    li_win_t win, li_win_msg_t action, li_key_code_t key, int x, int y,
    li_key_state_t state) {
    if (li_win_mouse_fun != NULL) {
        li_win_mouse_fun(win, action, key, x, y, state);
    }
}

void li_win_send_resize(
    li_win_t win, li_win_msg_t action, int width, int height) {
    if (li_win_resize_fun != NULL) {
        li_win_resize_fun(win, action, width, height);
    }
}

void li_win_send_close(li_win_t win, li_win_msg_t action) {
    if (li_win_close_fun != NULL) {
        li_win_close_fun(win, action);
    }
}
