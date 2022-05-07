#include "li/win/win.h"

#include "li/std.h"

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
li_win_fun_t              li_win_fun  = NULL;
li_win_t                  li_win_win  = NULL;
li_win_key_t              li_win_key  = 0;

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
