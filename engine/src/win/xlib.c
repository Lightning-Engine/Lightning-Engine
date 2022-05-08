#include "li/win/xlib.h"

#include "li/std.h"

Display *li_win_xlib_disp;
Window   li_win_xlib_root;
XContext li_win_xlib_context;
Atom     li_win_xlib_wm_delete_window;

const struct li_win_impl li_win_xlib_impl = {
    li_win_xlib_exit, li_win_xlib_poll, li_win_xlib_create, li_win_xlib_destroy
};

int li_win_xlib_init(void) {
    li_win_xlib_disp = XOpenDisplay(NULL);
    if (li_win_xlib_disp != NULL) {
        li_win_xlib_root    = XDefaultRootWindow(li_win_xlib_disp);
        li_win_xlib_context = XUniqueContext();
        li_win_xlib_wm_delete_window =
            XInternAtom(li_win_xlib_disp, "WM_DELETE_WINDOW", False);
        if (li_win_xlib_wm_delete_window != None) {
            li_win_impl = &li_win_xlib_impl;
            return 0;
        }
        XCloseDisplay(li_win_xlib_disp);
    }
    return -1;
}

void li_win_xlib_exit(void) {
    XCloseDisplay(li_win_xlib_disp);
}

void li_win_xlib_poll(void) {
    XEvent event;
    while (XPending(li_win_xlib_disp)) {
        XNextEvent(li_win_xlib_disp, &event);
        li_win_xlib_event(&event);
    }
}

li_win_t li_win_xlib_create(int width, int height) {
    struct li_win_xlib *win_xlib;
    win_xlib = li_std_malloc(sizeof *win_xlib);
    if (win_xlib != NULL) {
        win_xlib->window = XCreateSimpleWindow(
            li_win_xlib_disp, li_win_xlib_root, 0, 0, width, height, 0, 0, 0);
        if (win_xlib->window != 0) {
            XSelectInput(
                li_win_xlib_disp, win_xlib->window,
                KeyPressMask | KeyReleaseMask | ButtonPressMask
                    | ButtonReleaseMask | PointerMotionMask
                    | StructureNotifyMask);
            XSetWMProtocols(
                li_win_xlib_disp, win_xlib->window,
                &li_win_xlib_wm_delete_window, 1);
            XSaveContext(
                li_win_xlib_disp, win_xlib->window, li_win_xlib_context,
                (XPointer) win_xlib);
            XMapWindow(li_win_xlib_disp, win_xlib->window);
            return (li_win_t) win_xlib;
        }
        li_std_free(win_xlib);
    }
    return NULL;
}

void li_win_xlib_destroy(li_win_t win) {
    struct li_win_xlib *win_xlib = (struct li_win_xlib *) win;
    XDestroyWindow(li_win_xlib_disp, win_xlib->window);
    li_std_free(win_xlib);
}

int li_win_xlib_repeat(XEvent *event) {
    XEvent next_event;
    if (XPending(li_win_xlib_disp)) {
        XPeekEvent(li_win_xlib_disp, &next_event);
        if (next_event.type == KeyPress
            && next_event.xkey.time == event->xkey.time
            && next_event.xkey.keycode == event->xkey.keycode) {
            XNextEvent(li_win_xlib_disp, &next_event);
            return 1;
        }
    }
    return 0;
}

li_win_state_t li_win_xlib_state(unsigned int s) {
    li_win_state_t state = 0;
    if (s & ShiftMask) {
        state |= LI_KEY_STATE_SHIFT;
    }
    if (s & LockMask) {
        state |= LI_KEY_STATE_CAPSLOCK;
    }
    if (s & ControlMask) {
        state |= LI_KEY_STATE_CONTROL;
    }
    if (s & Mod1Mask) {
        state |= LI_KEY_STATE_ALT;
    }
    if (s & Mod2Mask) {
        state |= LI_KEY_STATE_NUMLOCK;
    }
    if (s & Mod4Mask) {
        state |= LI_KEY_STATE_SUPER;
    }
    if (s & Button1Mask) {
        state |= LI_KEY_STATE_LMOUSE;
    }
    if (s & Button3Mask) {
        state |= LI_KEY_STATE_RMOUSE;
    }
    if (s & Button2Mask) {
        state |= LI_KEY_STATE_MMOUSE;
    }
    return state;
}

li_win_key_t li_win_xlib_button(unsigned int button) {
    switch (button) {
    case Button1:
        return LI_KEY_CODE_LMOUSE;
    case Button3:
        return LI_KEY_CODE_RMOUSE;
    case Button2:
        return LI_KEY_CODE_MMOUSE;
    }
    return LI_KEY_CODE_NULL;
}

li_win_key_t li_win_xlib_key_impl(KeySym sym) {
    switch (sym) {
    case XK_0:
        return LI_KEY_CODE_0;
    case XK_1:
        return LI_KEY_CODE_1;
    case XK_2:
        return LI_KEY_CODE_2;
    case XK_3:
        return LI_KEY_CODE_3;
    case XK_4:
        return LI_KEY_CODE_4;
    case XK_5:
        return LI_KEY_CODE_5;
    case XK_6:
        return LI_KEY_CODE_6;
    case XK_7:
        return LI_KEY_CODE_7;
    case XK_8:
        return LI_KEY_CODE_8;
    case XK_9:
        return LI_KEY_CODE_9;
    case XK_A:
        return LI_KEY_CODE_A;
    case XK_B:
        return LI_KEY_CODE_B;
    case XK_C:
        return LI_KEY_CODE_C;
    case XK_D:
        return LI_KEY_CODE_D;
    case XK_E:
        return LI_KEY_CODE_E;
    case XK_F:
        return LI_KEY_CODE_F;
    case XK_G:
        return LI_KEY_CODE_G;
    case XK_H:
        return LI_KEY_CODE_H;
    case XK_I:
        return LI_KEY_CODE_I;
    case XK_J:
        return LI_KEY_CODE_J;
    case XK_K:
        return LI_KEY_CODE_K;
    case XK_L:
        return LI_KEY_CODE_L;
    case XK_M:
        return LI_KEY_CODE_M;
    case XK_N:
        return LI_KEY_CODE_N;
    case XK_O:
        return LI_KEY_CODE_O;
    case XK_P:
        return LI_KEY_CODE_P;
    case XK_Q:
        return LI_KEY_CODE_Q;
    case XK_R:
        return LI_KEY_CODE_R;
    case XK_S:
        return LI_KEY_CODE_S;
    case XK_T:
        return LI_KEY_CODE_T;
    case XK_U:
        return LI_KEY_CODE_U;
    case XK_V:
        return LI_KEY_CODE_V;
    case XK_W:
        return LI_KEY_CODE_W;
    case XK_X:
        return LI_KEY_CODE_X;
    case XK_Y:
        return LI_KEY_CODE_Y;
    case XK_Z:
        return LI_KEY_CODE_Z;
    case XK_KP_0:
        return LI_KEY_CODE_NUM0;
    case XK_KP_1:
        return LI_KEY_CODE_NUM1;
    case XK_KP_2:
        return LI_KEY_CODE_NUM2;
    case XK_KP_3:
        return LI_KEY_CODE_NUM3;
    case XK_KP_4:
        return LI_KEY_CODE_NUM4;
    case XK_KP_5:
        return LI_KEY_CODE_NUM5;
    case XK_KP_6:
        return LI_KEY_CODE_NUM6;
    case XK_KP_7:
        return LI_KEY_CODE_NUM7;
    case XK_KP_8:
        return LI_KEY_CODE_NUM8;
    case XK_KP_9:
        return LI_KEY_CODE_NUM9;
    case XK_KP_Decimal:
        return LI_KEY_CODE_NUMDOT;
    case XK_KP_Add:
        return LI_KEY_CODE_NUMPLUS;
    case XK_KP_Subtract:
        return LI_KEY_CODE_NUMMINUS;
    case XK_KP_Multiply:
        return LI_KEY_CODE_NUMSTAR;
    case XK_KP_Divide:
        return LI_KEY_CODE_NUMSLASH;
    case XK_KP_Enter:
        return LI_KEY_CODE_NUMENTER;
    case XK_Num_Lock:
        return LI_KEY_CODE_NUMLOCK;
    case XK_F1:
        return LI_KEY_CODE_F1;
    case XK_F2:
        return LI_KEY_CODE_F2;
    case XK_F3:
        return LI_KEY_CODE_F3;
    case XK_F4:
        return LI_KEY_CODE_F4;
    case XK_F5:
        return LI_KEY_CODE_F5;
    case XK_F6:
        return LI_KEY_CODE_F6;
    case XK_F7:
        return LI_KEY_CODE_F7;
    case XK_F8:
        return LI_KEY_CODE_F8;
    case XK_F9:
        return LI_KEY_CODE_F9;
    case XK_F10:
        return LI_KEY_CODE_F10;
    case XK_F11:
        return LI_KEY_CODE_F11;
    case XK_F12:
        return LI_KEY_CODE_F12;
    case XK_Print:
        return LI_KEY_CODE_PRINTSCR;
    case XK_Scroll_Lock:
        return LI_KEY_CODE_SCRLOCK;
    case XK_Pause:
        return LI_KEY_CODE_PAUSE;
    case XK_Insert:
        return LI_KEY_CODE_INSERT;
    case XK_Home:
        return LI_KEY_CODE_HOME;
    case XK_Page_Up:
        return LI_KEY_CODE_PAGEUP;
    case XK_Delete:
        return LI_KEY_CODE_DELETE;
    case XK_End:
        return LI_KEY_CODE_END;
    case XK_Page_Down:
        return LI_KEY_CODE_PAGEDOWN;
    case XK_Up:
        return LI_KEY_CODE_UP;
    case XK_Down:
        return LI_KEY_CODE_DOWN;
    case XK_Left:
        return LI_KEY_CODE_LEFT;
    case XK_Right:
        return LI_KEY_CODE_RIGHT;
    case XK_Escape:
        return LI_KEY_CODE_ESCAPE;
    case XK_BackSpace:
        return LI_KEY_CODE_BSPACE;
    case XK_Tab:
        return LI_KEY_CODE_TAB;
    case XK_Shift_L:
        return LI_KEY_CODE_LSHIFT;
    case XK_Shift_R:
        return LI_KEY_CODE_RSHIFT;
    case XK_Control_L:
        return LI_KEY_CODE_LCONTROL;
    case XK_Control_R:
        return LI_KEY_CODE_RCONTROL;
    case XK_Super_L:
        return LI_KEY_CODE_LSUPER;
    case XK_Super_R:
        return LI_KEY_CODE_RSUPER;
    case XK_Alt_L:
        return LI_KEY_CODE_LALT;
    case XK_Alt_R:
        return LI_KEY_CODE_RALT;
    case XK_Caps_Lock:
        return LI_KEY_CODE_CAPSLOCK;
    case XK_Return:
        return LI_KEY_CODE_ENTER;
    case XK_space:
        return LI_KEY_CODE_SPACE;
    case XK_period:
        return LI_KEY_CODE_DOT;
    case XK_comma:
        return LI_KEY_CODE_COMMA;
    case XK_semicolon:
        return LI_KEY_CODE_COLON;
    case XK_apostrophe:
        return LI_KEY_CODE_QUOTE;
    case XK_bracketleft:
        return LI_KEY_CODE_LBRACKET;
    case XK_bracketright:
        return LI_KEY_CODE_RBRACKET;
    case XK_backslash:
        return LI_KEY_CODE_BSLASH;
    case XK_minus:
        return LI_KEY_CODE_MINUS;
    case XK_slash:
        return LI_KEY_CODE_SLASH;
    case XK_grave:
        return LI_KEY_CODE_TILDE;
    case XK_equal:
        return LI_KEY_CODE_EQUAL;
    }
    return LI_KEY_CODE_NULL;
}

li_win_key_t li_win_xlib_key(unsigned int keycode) {
    int          i;
    int          count;
    li_win_key_t key = LI_KEY_CODE_NULL;
    KeySym *keysym = XGetKeyboardMapping(li_win_xlib_disp, keycode, 1, &count);
    for (i = 0; i < count && keysym[i] != NoSymbol; i++) {
        key = li_win_xlib_key_impl(keysym[i]);
        if (key != LI_KEY_CODE_NULL) {
            break;
        }
    }
    XFree(keysym);
    return key;
}

void li_win_xlib_event(XEvent *event) {
    XPointer win;
    XFindContext(
        li_win_xlib_disp, event->xany.window, li_win_xlib_context, &win);
    li_win_win = (li_win_t) win;
    switch (event->type) {
    case ClientMessage:
        if ((unsigned long) event->xclient.data.l[0]
            == li_win_xlib_wm_delete_window) {
            li_win_fun(li_win_msg_close);
        }
        break;
    case KeyPress:
    case KeyRelease:
        li_win_key        = li_win_xlib_key(event->xkey.keycode);
        li_win_win->state = li_win_xlib_state(event->xkey.state);
        if (event->type == KeyPress) {
            li_win_fun(li_win_msg_keydown);
        } else if (li_win_xlib_repeat(event)) {
            li_win_fun(li_win_msg_keyrep);
        } else {
            li_win_fun(li_win_msg_keyup);
        }
        break;
    case ButtonPress:
    case ButtonRelease:
        li_win_key         = li_win_xlib_button(event->xbutton.button);
        li_win_win->state  = li_win_xlib_state(event->xbutton.state);
        li_win_win->mousex = event->xbutton.x;
        li_win_win->mousey = event->xbutton.y;
        if (event->type == ButtonPress) {
            li_win_fun(li_win_msg_mousedown);
        } else {
            li_win_fun(li_win_msg_mouseup);
        }
        break;
    case MotionNotify:
        li_win_win->state  = li_win_xlib_state(event->xmotion.state);
        li_win_win->mousex = event->xmotion.x;
        li_win_win->mousey = event->xmotion.y;
        li_win_fun(li_win_msg_mousemove);
        break;
    case ConfigureNotify:
        li_win_win->width  = event->xconfigure.width;
        li_win_win->height = event->xconfigure.height;
        li_win_fun(li_win_msg_resize);
        break;
    }
}
