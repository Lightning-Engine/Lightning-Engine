#include "li/win.h"
#include <windows.h>

const static char *LI_DEFAULT_CLASS_NAME = "LIWINDOW";
static HINSTANCE LI_WIN_HANDLE;

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

int li_win_init(void) {
    WNDCLASS wndClass = { };

    LI_WIN_HANDLE = GetModuleHandleA(NULL);
    wndClass.hInstance = LI_WIN_HANDLE;
    wndClass.lpszClassName = LI_DEFAULT_CLASS_NAME;
    wndClass.lpfnWndProc = winProc;

    if (RegisterClass(&wndClass) == 0)
        return -1;
}

void li_win_exit(void) {

}

void li_win_poll(void) {

}

int li_win_create(li_win_t *win, int width, int height) {
    win->p = CreateWindowExA(0, LI_DEFAULT_CLASS_NAME, "New Window", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, LI_WIN_HANDLE, NULL);
    if (win->p == 0)
        return -1;
    ShowWindow(win->p, SW_SHOW);
}

void li_win_destroy(li_win_t win) {
    DestroyWindow(win.p);
}

void li_win_map(li_win_t win) {

}

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, msg, w_param, l_param);
}
