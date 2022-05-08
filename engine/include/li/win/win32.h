#ifndef LI_WIN_WIN32_H
#define LI_WIN_WIN32_H

#include "li/win/impl.h"

#include <windows.h>

struct li_win_win32 {
    struct li_win_base base;
    HWND               hwnd;
};

extern HINSTANCE li_win_win32_instance;

int      li_win_win32_init(void);
void     li_win_win32_exit(void);
void     li_win_win32_poll(void);
li_win_t li_win_win32_create(int width, int height);
void     li_win_win32_destroy(li_win_t win);

li_input_state_t li_win_win32_xlat_state(void);
li_input_key_t   li_win_win32_xlat_button(UINT umsg);
li_input_key_t   li_win_win32_xlat_key(LPARAM lparam);

void li_win_win32_event_key(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam);
void li_win_win32_event_button(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam);
void li_win_win32_event_motion(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam);
void li_win_win32_event_size(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam);
void li_win_win32_event_close(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam);
LRESULT CALLBACK
li_win_win32_event(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

#endif
