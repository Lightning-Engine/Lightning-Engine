#ifndef LI_WIN_IMPL_H
#define LI_WIN_IMPL_H

#include "li/win/win.h"

struct li_win_impl {
    int (*init)(void);
    void (*exit)(void);
    void (*poll)(void);
    li_win_t (*create)(int width, int height);
    void (*destroy)(li_win_t win);
};

struct li_win_base {
    struct li_win win;
};

extern const struct li_dl_impl   li_dl_xlib_impl;
extern const struct li_dl_impl   li_dl_win32_impl;
extern const struct li_dl_impl   li_dl_cocoa_impl;
extern const struct li_win_impl *li_win_impl;

void li_win_send_key(
    li_win_t win, li_win_msg_t action, li_key_code_t key, li_key_state_t state);
void li_win_send_mouse(
    li_win_t win, li_win_msg_t action, li_key_code_t key, int x, int y,
    li_key_state_t state);
void li_win_send_resize(
    li_win_t win, li_win_msg_t action, int width, int height);
void li_win_send_close(li_win_t win, li_win_msg_t action);

#endif
