#ifndef LI_WIN_WIN32_H
#define LI_WIN_WIN32_H

#include "li/win/impl.h"

#include <windows.h>

struct li_win_win32 {
    HWND hwnd;
};

extern HINSTANCE li_win_win32_instance;

int      li_win_win32_init(void);
void     li_win_win32_exit(void);
void     li_win_win32_poll(void);
li_win_t li_win_win32_create(int width, int height);
void     li_win_win32_destroy(li_win_t win);

void li_win_win32_event_key(li_win_t win, LPARAM lparam, int down);
void li_win_win32_event_button(
    li_win_t win, LPARAM lparam, int down, li_input_button_t button);
void li_win_win32_event_motion(li_win_t win, LPARAM lparam);
void li_win_win32_event_size(li_win_t win, LPARAM lparam);
void li_win_win32_event_close(li_win_t win);
LRESULT CALLBACK
li_win_win32_event(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

li_input_state_t li_win_win32_get_state(void);
li_input_key_t   li_win_win32_get_key(LPARAM lparam);

#endif
