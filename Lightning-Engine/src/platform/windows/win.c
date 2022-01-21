#include "li/win.h"
#include <windows.h>

const static wchar_t *LI_DEFAULT_CLASS_NAME = L"LIWINDOW";
static HINSTANCE LI_WIN_HANDLE;

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

int li_win_init(void) {
    WNDCLASS wndClass = { };

    LI_WIN_HANDLE = GetModuleHandle(NULL);
    wndClass.hInstance = LI_WIN_HANDLE;
    wndClass.lpszClassName = LI_DEFAULT_CLASS_NAME;
    wndClass.lpfnWndProc = winProc;

    if (RegisterClass(&wndClass) == 0)
        return -1;
}

void li_win_exit(void) {

}

void li_win_poll(void) {
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

int li_win_create(li_win_t *win, int width, int height) {
    win->p = CreateWindowExW(0, LI_DEFAULT_CLASS_NAME, L"New Window", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, LI_WIN_HANDLE, NULL);
    if (win->p == 0)
        return -1;
    ShowWindow(win->p, SW_SHOW);
}

void _li_win_destroy(void *win_ptr) {
    DestroyWindow(win_ptr);
}

void li_win_destroy(li_win_t win) {
    _li_win_destroy(win.p);
}

void li_win_map(li_win_t win) {

}

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_CLOSE:
            _li_win_destroy(hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, msg, w_param, l_param);
}
