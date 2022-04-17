#include "li_win.h"

#include <stdlib.h>

struct li_win_impl {
    int (*poll)(void);
    li_win_t (*create)(int width, int height);
    int (*destroy)(li_win_t win);
};

static li_win_fun_t              li_win_fun;
static const struct li_win_impl *li_win_impl;

#if LI_WIN_XLIB
# include <X11/Xlib.h>

struct li_win_xlib {
    Window window;
};

static int      li_win_init_xlib(void);
static int      li_win_poll_xlib(void);
static li_win_t li_win_create_xlib(int width, int height);
static int      li_win_destroy_xlib(li_win_t win);
static void     li_win_event_xlib(XEvent *event);

static Display *li_win_display_xlib;
static Window   li_win_root_xlib;

static const struct li_win_impl li_win_impl_xlib = { li_win_poll_xlib,
                                                     li_win_create_xlib,
                                                     li_win_destroy_xlib };

static int li_win_init_xlib(void) {
    li_win_display_xlib = XOpenDisplay(NULL);
    if (li_win_display_xlib != NULL) {
        li_win_root_xlib = XDefaultRootWindow(li_win_display_xlib);
        li_win_impl      = &li_win_impl_xlib;
        return 0;
    }
    return -1;
}

static int li_win_poll_xlib(void) {
    XEvent event;
    while (XPending(li_win_display_xlib)) {
        XNextEvent(li_win_display_xlib, &event);
        li_win_event_xlib(&event);
    }
    return 0;
}

static li_win_t li_win_create_xlib(int width, int height) {
    struct li_win_xlib *win_xlib;
    win_xlib = malloc(sizeof *win_xlib);
    if (win_xlib != NULL) {
        win_xlib->window = XCreateSimpleWindow(
            li_win_display_xlib, li_win_root_xlib, 0, 0, width, height, 0, 0,
            0);
        if (win_xlib->window != 0) {
            XMapWindow(li_win_display_xlib, win_xlib->window);
            return win_xlib;
        }
        free(win_xlib);
    }
    return NULL;
}

static int li_win_destroy_xlib(li_win_t win) {
    struct li_win_xlib *win_xlib = win;
    XDestroyWindow(li_win_display_xlib, win_xlib->window);
    free(win_xlib);
    return 0;
}

static void li_win_event_xlib(XEvent *event) {
    li_win_fun();
}

#endif

#if LI_WIN_WIN32
# include <windows.h>

# ifndef LI_WIN_CLASS_WIN32
#  define LI_WIN_CLASS_WIN32 TEXT("LIWIN")
# endif
# ifndef LI_WIN_TITLE_WIN32
#  define LI_WIN_TITLE_WIN32 TEXT("New Window")
# endif

struct li_win_win32 {
    HWND handle;
    HDC  dc;
};

static int      li_win_init_win32(void);
static int      li_win_poll_win32(void);
static li_win_t li_win_create_win32(int width, int height);
static int      li_win_destroy_win32(li_win_t win);
static LRESULT CALLBACK
li_win_event_win32(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

static HINSTANCE li_win_instance_win32;

static const struct li_win_impl li_win_impl_win32 = { li_win_poll_win32,
                                                      li_win_create_win32,
                                                      li_win_destroy_win32 };

static int li_win_init_win32(void) {
    WNDCLASS wndclass     = { 0 };
    li_win_impl           = &li_win_impl_win32;
    li_win_instance_win32 = GetModuleHandle(NULL);
    if (li_win_instance_win32 != NULL) {
        wndclass.hInstance     = li_win_instance_win32;
        wndclass.lpszClassName = LI_WIN_CLASS_WIN32;
        wndclass.lpfnWndProc   = li_win_event_win32;
        wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
        if (wndclass.hCursor != NULL) {
            if (RegisterClass(&wndclass)) {
                return 0;
            }
            DestroyCursor(wndclass.hCursor);
        }
    }
    return -1;
}

static li_win_t li_win_create_win32(int width, int height) {
    struct li_win_win32 *win_win32;
    win_win32 = malloc(sizeof *win_win32);
    if (win_win32 != NULL) {
        win_win32->handle = CreateWindowEx(
            CS_OWNDC, LI_WIN_CLASS_WIN32, LI_WIN_TITLE_WIN32,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, li_win_instance_win32, NULL);
        if (win_win32->handle != NULL) {
            win_win32->dc = GetDC(win_win32->handle);
            if (win_win32->dc != NULL) {
                ShowWindow(win_win32->handle, SW_SHOW);
                return win_win32;
            }
            DestroyWindow(win_win32->handle);
        }
        free(win_win32);
    }
    return NULL;
}

static int li_win_destroy_win32(li_win_t win) {
    struct li_win_win32 *win_win32 = win;
    ReleaseDC(win_win32->handle, win_win32->dc);
    DestroyWindow(win_win32->handle);
    free(win_win32);
    return 0;
}

static int li_win_poll_win32(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

static LRESULT CALLBACK
li_win_event_win32(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    (void) hwnd;
    (void) w_param;
    (void) l_param;
    switch (msg) {
    default:
        return DefWindowProc(hwnd, msg, w_param, l_param);
    }
}

#endif

#if LI_WIN_COCOA
# include <Cocoa/Cocoa.h>

#endif

int li_win_init(li_win_fun_t fun) {
    int result = -1;
    li_win_fun = fun;
#if LI_WIN_XLIB
    if (result == -1) {
        result = li_win_init_xlib();
    }
#endif
#if LI_WIN_WIN32
    if (result == -1) {
        result = li_win_init_win32();
    }
#endif
#if LI_WIN_COCOA
    if (result == -1) {
        result = li_win_init_cocoa();
    }
#endif
    return result;
}

int li_win_poll(void) {
    return li_win_impl->poll();
}

li_win_t li_win_create(int width, int height) {
    return li_win_impl->create(width, height);
}

int li_win_destroy(li_win_t win) {
    return li_win_impl->destroy(win);
}
