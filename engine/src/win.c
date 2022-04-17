#include "li_win.h"

#include <stdlib.h>

struct li_win_impl {
    void (*poll)(void);
    li_win_t (*create)(int width, int height);
    void (*destroy)(li_win_t win);
};

static li_win_fun_t              li_win_fun;
static const struct li_win_impl *li_win_impl;

#if LI_WIN_XLIB
# include <X11/Xlib.h>

struct li_win_xlib {
    Window window;
};

static int      li_win_init_xlib(void);
static void     li_win_exit_xlib(void);
static void     li_win_poll_xlib(void);
static li_win_t li_win_create_xlib(int width, int height);
static void     li_win_destroy_xlib(li_win_t win);
static void     li_win_event_xlib(XEvent *event);

static Display *li_win_disp_xlib;
static Window   li_win_root_xlib;

static const struct li_win_impl li_win_impl_xlib = { li_win_poll_xlib,
                                                     li_win_create_xlib,
                                                     li_win_destroy_xlib };

static int li_win_init_xlib(void) {
    li_win_disp_xlib = XOpenDisplay(NULL);
    if (li_win_disp_xlib != NULL) {
        li_win_root_xlib = XDefaultRootWindow(li_win_disp_xlib);
        li_win_impl      = &li_win_impl_xlib;
        atexit(li_win_exit_xlib);
        return 0;
    }
    return -1;
}

static void li_win_exit_xlib(void) {
    XCloseDisplay(li_win_disp_xlib);
}

static void li_win_poll_xlib(void) {
    XEvent event;
    while (XPending(li_win_disp_xlib)) {
        XNextEvent(li_win_disp_xlib, &event);
        li_win_event_xlib(&event);
    }
}

static li_win_t li_win_create_xlib(int width, int height) {
    struct li_win_xlib *win_xlib;
    win_xlib = malloc(sizeof *win_xlib);
    if (win_xlib != NULL) {
        win_xlib->window = XCreateSimpleWindow(
            li_win_disp_xlib, li_win_root_xlib, 0, 0, width, height, 0, 0, 0);
        if (win_xlib->window != 0) {
            XMapWindow(li_win_disp_xlib, win_xlib->window);
            return win_xlib;
        }
        free(win_xlib);
    }
    return NULL;
}

static void li_win_destroy_xlib(li_win_t win) {
    struct li_win_xlib *win_xlib = win;
    XDestroyWindow(li_win_disp_xlib, win_xlib->window);
    free(win_xlib);
}

static void li_win_event_xlib(XEvent *event) {
    li_win_fun();
}

#endif

#if LI_WIN_WIN32
# include <windows.h>

# ifndef LI_WIN_CLASS_WIN32
#  define LI_WIN_CLASS_WIN32 "LI_WIN"
# endif
# ifndef LI_WIN_TITLE_WIN32
#  define LI_WIN_TITLE_WIN32 "Lightning"
# endif

struct li_win_win32 {
    HWND hwnd;
};

static int      li_win_init_win32(void);
static void     li_win_exit_win32(void);
static void     li_win_poll_win32(void);
static li_win_t li_win_create_win32(int width, int height);
static void     li_win_destroy_win32(li_win_t win);
static LRESULT CALLBACK
li_win_event_win32(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

static HINSTANCE li_win_instance_win32;

static const struct li_win_impl li_win_impl_win32 = { li_win_poll_win32,
                                                      li_win_create_win32,
                                                      li_win_destroy_win32 };

static int li_win_init_win32(void) {
    WNDCLASS wndclass      = { 0 };
    li_win_impl            = &li_win_impl_win32;
    li_win_instance_win32  = GetModuleHandle(NULL);
    wndclass.hInstance     = li_win_instance_win32;
    wndclass.lpszClassName = LI_WIN_CLASS_WIN32;
    wndclass.lpfnWndProc   = li_win_event_win32;
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wndclass);
    atexit(li_win_exit_win32);
    return 0;
}

static void li_win_exit_win32(void) {
    UnregisterClass(LI_WIN_CLASS_WIN32, li_win_instance_win32);
}

static void li_win_poll_win32(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

static li_win_t li_win_create_win32(int width, int height) {
    struct li_win_win32 *win_win32;
    win_win32 = malloc(sizeof *win_win32);
    if (win_win32 != NULL) {
        win_win32->hwnd = CreateWindow(
            LI_WIN_CLASS_WIN32, LI_WIN_TITLE_WIN32, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL,
            li_win_instance_win32, NULL);
        if (win_win32->hwnd != NULL) {
            ShowWindow(win_win32->hwnd, SW_SHOW);
            return win_win32;
        }
        free(win_win32);
    }
    return NULL;
}

static void li_win_destroy_win32(li_win_t win) {
    struct li_win_win32 *win_win32 = win;
    DestroyWindow(win_win32->hwnd);
    free(win_win32);
}

static LRESULT CALLBACK
li_win_event_win32(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}

#endif

#if LI_WIN_COCOA
# include <objc/objc-runtime.h>
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
#if LI_WIN_COCOA && 0
    if (result == -1) {
        result = li_win_init_cocoa();
    }
#endif
    return result;
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
