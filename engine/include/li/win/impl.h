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

extern const struct li_win_impl  li_win_xlib_impl;
extern const struct li_win_impl  li_win_win32_impl;
extern const struct li_win_impl  li_win_cocoa_impl;
extern const struct li_win_impl *li_win_impl;

void li_win_send_key(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, li_input_key_t key);
void li_win_send_button(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y,
    li_input_button_t button);
void li_win_send_motion(
    li_win_t win, li_win_msg_t msg, li_input_state_t state, int x, int y);
void li_win_send_size(li_win_t win, li_win_msg_t msg, int width, int height);
void li_win_send_close(li_win_t win, li_win_msg_t msg);

#endif
