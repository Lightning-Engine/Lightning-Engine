#include "li/win/win32.h"

#include "li/std.h"

#ifndef LI_WIN_WIN32_CLASS
# define LI_WIN_WIN32_CLASS "LI_WIN"
#endif
#ifndef LI_WIN_WIN32_TITLE
# define LI_WIN_WIN32_TITLE "Lightning"
#endif

#define MOUSEX(lparam)    (LOWORD(lparam))
#define MOUSEY(lparam)    (HIWORD(lparam))
#define WINWIDTH(lparam)  (LOWORD(lparam))
#define WINHEIGHT(lparam) (HIWORD(lparam))
#define ISREPEAT(lparam)  (!!(lparam & (1 << 30)))
#define ISKEYUP(umsg)     (umsg == WM_KEYUP || umsg == WM_SYSKEYUP)
#define ISMOUSEMOVE(umsg) (umsg == WM_MOUSEMOVE)
#define ISBUTTONDOWN(umsg)                                                     \
 (umsg == WM_LBUTTONDOWN || umsg == WM_MBUTTONDOWN || umsg == WM_RBUTTONDOWN   \
  || umsg == WM_XBUTTONDOWN)

HINSTANCE li_win_win32_instance;
WNDCLASS  li_win_win32_class;

const struct li_win_impl li_win_win32_impl = {
    li_win_win32_init, li_win_win32_exit, li_win_win32_poll,
    li_win_win32_create, li_win_win32_destroy
};

int li_win_win32_init(void) {
    li_win_impl                      = &li_win_win32_impl;
    li_win_win32_instance            = GetModuleHandle(NULL);
    li_win_win32_class.hInstance     = li_win_win32_instance;
    li_win_win32_class.lpszClassName = LI_WIN_WIN32_CLASS;
    li_win_win32_class.lpfnWndProc   = li_win_win32_event;
    li_win_win32_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&li_win_win32_class);
    return 0;
}

void li_win_win32_exit(void) {
    UnregisterClass(LI_WIN_WIN32_CLASS, li_win_win32_instance);
}

void li_win_win32_poll(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

li_win_t li_win_win32_create(int width, int height) {
    struct li_win_win32 *win_win32;
    win_win32 = li_std_malloc(sizeof *win_win32);
    if (win_win32 != NULL) {
        win_win32->hwnd = CreateWindow(
            LI_WIN_WIN32_CLASS, LI_WIN_WIN32_TITLE, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL,
            li_win_win32_instance, NULL);
        if (win_win32->hwnd != NULL) {
            SetWindowLongPtr(
                win_win32->hwnd, GWLP_USERDATA, (LONG_PTR) win_win32);
            ShowWindow(win_win32->hwnd, SW_SHOW);
            return (li_win_t) win_win32;
        }
        li_std_free(win_win32);
    }
    return NULL;
}

void li_win_win32_destroy(li_win_t win) {
    struct li_win_win32 *win_win32 = (struct li_win_win32 *) win;
    DestroyWindow(win_win32->hwnd);
    li_std_free(win_win32);
}

li_input_state_t li_win_win32_xlat_state(void) {
    li_input_state_t state = 0;
    if (GetKeyState(VK_SHIFT) & 0x8000) {
        state |= LI_INPUT_STATE_SHIFT;
    }
    if (GetKeyState(VK_CONTROL) & 0x8000) {
        state |= LI_INPUT_STATE_CONTROL;
    }
    if (GetKeyState(VK_MENU) & 0x8000) {
        state |= LI_INPUT_STATE_ALT;
    }
    if (GetKeyState(VK_NUMLOCK) & 0x0001) {
        state |= LI_INPUT_STATE_NUMLOCK;
    }
    if (GetKeyState(VK_CAPITAL) & 0x0001) {
        state |= LI_INPUT_STATE_CAPSLOCK;
    }
    if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) {
        state |= LI_INPUT_STATE_SUPER;
    }
    if (GetKeyState(VK_LBUTTON) & 0x8000) {
        state |= LI_INPUT_STATE_LMOUSE;
    }
    if (GetKeyState(VK_RBUTTON) & 0x8000) {
        state |= LI_INPUT_STATE_RMOUSE;
    }
    if (GetKeyState(VK_MBUTTON) & 0x8000) {
        state |= LI_INPUT_STATE_MMOUSE;
    }
    return state;
}

li_input_key_t li_win_win32_xlat_key(LPARAM lparam) {
    switch ((lparam & 0x1FF0000) >> 16) {
    case 0x00B:
        return LI_INPUT_KEY_0;
    case 0x002:
        return LI_INPUT_KEY_1;
    case 0x003:
        return LI_INPUT_KEY_2;
    case 0x004:
        return LI_INPUT_KEY_3;
    case 0x005:
        return LI_INPUT_KEY_4;
    case 0x006:
        return LI_INPUT_KEY_5;
    case 0x007:
        return LI_INPUT_KEY_6;
    case 0x008:
        return LI_INPUT_KEY_7;
    case 0x009:
        return LI_INPUT_KEY_8;
    case 0x00A:
        return LI_INPUT_KEY_9;
    case 0x01E:
        return LI_INPUT_KEY_A;
    case 0x030:
        return LI_INPUT_KEY_B;
    case 0x02E:
        return LI_INPUT_KEY_C;
    case 0x020:
        return LI_INPUT_KEY_D;
    case 0x012:
        return LI_INPUT_KEY_E;
    case 0x021:
        return LI_INPUT_KEY_F;
    case 0x022:
        return LI_INPUT_KEY_G;
    case 0x023:
        return LI_INPUT_KEY_H;
    case 0x017:
        return LI_INPUT_KEY_I;
    case 0x024:
        return LI_INPUT_KEY_J;
    case 0x025:
        return LI_INPUT_KEY_K;
    case 0x026:
        return LI_INPUT_KEY_L;
    case 0x032:
        return LI_INPUT_KEY_M;
    case 0x031:
        return LI_INPUT_KEY_N;
    case 0x018:
        return LI_INPUT_KEY_O;
    case 0x019:
        return LI_INPUT_KEY_P;
    case 0x010:
        return LI_INPUT_KEY_Q;
    case 0x013:
        return LI_INPUT_KEY_R;
    case 0x01F:
        return LI_INPUT_KEY_S;
    case 0x014:
        return LI_INPUT_KEY_T;
    case 0x016:
        return LI_INPUT_KEY_U;
    case 0x02F:
        return LI_INPUT_KEY_V;
    case 0x011:
        return LI_INPUT_KEY_W;
    case 0x02D:
        return LI_INPUT_KEY_X;
    case 0x015:
        return LI_INPUT_KEY_Y;
    case 0x02C:
        return LI_INPUT_KEY_Z;
    case 0x052:
        return LI_INPUT_KEY_NUM0;
    case 0x04F:
        return LI_INPUT_KEY_NUM1;
    case 0x050:
        return LI_INPUT_KEY_NUM2;
    case 0x051:
        return LI_INPUT_KEY_NUM3;
    case 0x04B:
        return LI_INPUT_KEY_NUM4;
    case 0x04C:
        return LI_INPUT_KEY_NUM5;
    case 0x04D:
        return LI_INPUT_KEY_NUM6;
    case 0x047:
        return LI_INPUT_KEY_NUM7;
    case 0x048:
        return LI_INPUT_KEY_NUM8;
    case 0x049:
        return LI_INPUT_KEY_NUM9;
    case 0x053:
        return LI_INPUT_KEY_NUMDOT;
    case 0x04E:
        return LI_INPUT_KEY_NUMPLUS;
    case 0x04A:
        return LI_INPUT_KEY_NUMMINUS;
    case 0x037:
        return LI_INPUT_KEY_NUMSTAR;
    case 0x135:
        return LI_INPUT_KEY_NUMSLASH;
    case 0x11C:
        return LI_INPUT_KEY_NUMENTER;
    case 0x145:
        return LI_INPUT_KEY_NUMLOCK;
    case 0x03B:
        return LI_INPUT_KEY_F1;
    case 0x03C:
        return LI_INPUT_KEY_F2;
    case 0x03D:
        return LI_INPUT_KEY_F3;
    case 0x03E:
        return LI_INPUT_KEY_F4;
    case 0x03F:
        return LI_INPUT_KEY_F5;
    case 0x040:
        return LI_INPUT_KEY_F6;
    case 0x041:
        return LI_INPUT_KEY_F7;
    case 0x042:
        return LI_INPUT_KEY_F8;
    case 0x043:
        return LI_INPUT_KEY_F9;
    case 0x044:
        return LI_INPUT_KEY_F10;
    case 0x057:
        return LI_INPUT_KEY_F11;
    case 0x058:
        return LI_INPUT_KEY_F12;
    case 0x137:
        return LI_INPUT_KEY_PRINTSCR;
    case 0x046:
        return LI_INPUT_KEY_SCRLOCK;
    case 0x045:
        return LI_INPUT_KEY_PAUSE;
    case 0x146:
        return LI_INPUT_KEY_PAUSE;
    case 0x152:
        return LI_INPUT_KEY_INSERT;
    case 0x147:
        return LI_INPUT_KEY_HOME;
    case 0x149:
        return LI_INPUT_KEY_PAGEUP;
    case 0x153:
        return LI_INPUT_KEY_DELETE;
    case 0x14F:
        return LI_INPUT_KEY_END;
    case 0x151:
        return LI_INPUT_KEY_PAGEDOWN;
    case 0x148:
        return LI_INPUT_KEY_UP;
    case 0x150:
        return LI_INPUT_KEY_DOWN;
    case 0x14B:
        return LI_INPUT_KEY_LEFT;
    case 0x14D:
        return LI_INPUT_KEY_RIGHT;
    case 0x001:
        return LI_INPUT_KEY_ESCAPE;
    case 0x00E:
        return LI_INPUT_KEY_BSPACE;
    case 0x00F:
        return LI_INPUT_KEY_TAB;
    case 0x02A:
        return LI_INPUT_KEY_LSHIFT;
    case 0x036:
        return LI_INPUT_KEY_RSHIFT;
    case 0x01D:
        return LI_INPUT_KEY_LCONTROL;
    case 0x11D:
        return LI_INPUT_KEY_RCONTROL;
    case 0x15B:
        return LI_INPUT_KEY_LSUPER;
    case 0x15C:
        return LI_INPUT_KEY_RSUPER;
    case 0x038:
        return LI_INPUT_KEY_LALT;
    case 0x138:
        return LI_INPUT_KEY_RALT;
    case 0x03A:
        return LI_INPUT_KEY_CAPSLOCK;
    case 0x01C:
        return LI_INPUT_KEY_ENTER;
    case 0x039:
        return LI_INPUT_KEY_SPACE;
    case 0x034:
        return LI_INPUT_KEY_DOT;
    case 0x033:
        return LI_INPUT_KEY_COMMA;
    case 0x027:
        return LI_INPUT_KEY_COLON;
    case 0x028:
        return LI_INPUT_KEY_QUOTE;
    case 0x01A:
        return LI_INPUT_KEY_LBRACKET;
    case 0x01B:
        return LI_INPUT_KEY_RBRACKET;
    case 0x02B:
        return LI_INPUT_KEY_BSLASH;
    case 0x00C:
        return LI_INPUT_KEY_MINUS;
    case 0x035:
        return LI_INPUT_KEY_SLASH;
    case 0x029:
        return LI_INPUT_KEY_TILDE;
    case 0x00D:
        return LI_INPUT_KEY_EQUAL;
    }
    return LI_INPUT_KEY_NULL;
}

li_input_key_t li_win_win32_xlat_button(UINT umsg) {
    switch (umsg) {
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        return LI_INPUT_BUTTON_LEFT;
    case WM_RBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        return LI_INPUT_BUTTON_RIGHT;
    case WM_MBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
        return LI_INPUT_BUTTON_MIDDLE;
    }
    return LI_INPUT_BUTTON_NULL;
}

void li_win_win32_event_key(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam) {
    li_input_state_t keystate;
    li_input_key_t   keycode;

    (void) wparam;
    keystate = li_win_win32_xlat_state();
    keycode  = li_win_win32_xlat_key(lparam);
    if (ISKEYUP(umsg)) {
        li_win_send_key(win, li_win_msg_key_up, keycode, keystate);
    } else if (ISREPEAT(lparam)) {
        li_win_send_key(win, li_win_msg_key_repeat, keycode, keystate);
    } else {
        li_win_send_key(win, li_win_msg_key_down, keycode, keystate);
    }
}

void li_win_win32_event_button(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam) {
    li_input_state_t keystate;
    li_input_key_t   keycode;
    int              x, y;

    (void) wparam;
    keystate = li_win_win32_xlat_state();
    keycode  = li_win_win32_xlat_button(umsg);
    x        = MOUSEX(lparam);
    y        = MOUSEY(lparam);

    if (ISBUTTONDOWN(umsg)) {
        li_win_send_button(
            win, li_win_msg_button_down, keystate, keycode, x, y);
    } else {
        li_win_send_button(win, li_win_msg_button_up, keystate, keycode, x, y);
    }
}

void li_win_win32_event_motion(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam) {
    li_input_state_t keystate;
    int              x, y;

    (void) umsg;
    (void) wparam;
    keystate = li_win_win32_xlat_state();
    x        = MOUSEX(lparam);
    y        = MOUSEY(lparam);

    li_win_send_motion(win, li_win_msg_motion, keystate, x, y);
}

void li_win_win32_event_size(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam) {
    int width, height;

    (void) umsg;
    (void) wparam;
    width  = WINWIDTH(lparam);
    height = WINHEIGHT(lparam);

    li_win_send_size(win, li_win_msg_size, width, height);
}

void li_win_win32_event_close(
    li_win_t win, UINT umsg, WPARAM wparam, LPARAM lparam) {
    (void) umsg;
    (void) wparam;
    (void) lparam;

    li_win_send_close(win, li_win_msg_close);
}

LRESULT CALLBACK
li_win_win32_event(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    li_win_t win;
    win = (li_win_t) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    switch (umsg) {
    case WM_CLOSE:
        li_win_win32_event_close(win, umsg, wparam, lparam);
        return 0;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        li_win_win32_event_key(win, umsg, wparam, lparam);
        return 0;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_XBUTTONUP:
        li_win_win32_event_button(win, umsg, wparam, lparam);
        return 0;
    case WM_MOUSEMOVE:
        li_win_win32_event_motion(win, umsg, wparam, lparam);
        return 0;
    case WM_SIZE:
        li_win_win32_event_size(win, umsg, wparam, lparam);
        return (0);
    default:
        return DefWindowProc(hwnd, umsg, wparam, lparam);
    }
}
