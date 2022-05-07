#ifndef LI_WIN_WIN32_H
#define LI_WIN_WIN32_H

#include "li/win/win.h"

#include <windows.h>

struct li_win_win32 {
    struct li_win base;
    HWND          hwnd;
};

extern HINSTANCE li_win_win32_instance;

extern const struct li_win_impl li_win_win32_impl;

int            li_win_win32_init(void);
void           li_win_win32_exit(void);
void           li_win_win32_poll(void);
li_win_t       li_win_win32_create(int width, int height);
void           li_win_win32_destroy(li_win_t win);
li_win_state_t li_win_win32_state(void);
li_win_key_t   li_win_win32_button(UINT umsg);
li_win_key_t   li_win_win32_key(LPARAM lparam);
LRESULT CALLBACK
li_win_win32_event(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

#endif
