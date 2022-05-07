#ifndef LI_WIN_COCOA_H
#define LI_WIN_COCOA_H

#include "li/win/win.h"

#include <objc/objc-runtime.h>

struct li_win_cocoa {
    struct li_win base;
    id            window;
    id            view;
    id            delegate;
};

extern const struct li_win_impl li_win_cocoa_impl;

int            li_win_cocoa_init(void);
void           li_win_cocoa_exit(void);
void           li_win_cocoa_poll(void);
li_win_t       li_win_cocoa_create(int width, int height);
void           li_win_cocoa_destroy(li_win_t win);
li_win_state_t li_win_cocoa_state(NSUInteger flags);
li_win_key_t   li_win_cocoa_button(NSUInteger type);
li_win_key_t   li_win_cocoa_key(unsigned short keycode);
void           li_win_cocoa_mouse(
              struct li_win_cocoa *win, li_win_state_t state, NSEvent *event);

#endif
