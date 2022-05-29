#include "li/win/xlib.h"

#include "li/std.h"

Display *li_win_xlib_disp;
Window   li_win_xlib_root;
XContext li_win_xlib_context;
Atom     li_win_xlib_wm_delete_window;

const struct li_win_impl li_win_xlib_impl = {
    li_win_xlib_init, li_win_xlib_exit, li_win_xlib_poll, li_win_xlib_create,
    li_win_xlib_destroy
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

void li_win_xlib_event_key(li_win_t win, XEvent *event, int down) {
    li_win_send_key(
        win,
        li_win_xlib_get_repeat(event) ? li_win_msg_key_repeat
        : down                        ? li_win_msg_key_down
                                      : li_win_msg_key_up,
        li_win_xlib_get_state(event->xkey.state),
        li_win_xlib_get_key(event->xkey.keycode));
}

void li_win_xlib_event_button(li_win_t win, XEvent *event, int down) {
    li_win_send_button(
        win, down ? li_win_msg_button_down : li_win_msg_button_up,
        li_win_xlib_get_state(event->xbutton.state), event->xbutton.x,
        event->xbutton.y, li_win_xlib_get_button(event->xbutton.button));
}

void li_win_xlib_event_motion(li_win_t win, XEvent *event) {
    li_win_send_motion(
        win, li_win_msg_motion, li_win_xlib_get_state(event->xmotion.state),
        event->xmotion.x, event->xmotion.y);
}

void li_win_xlib_event_size(li_win_t win, XEvent *event) {
    li_win_send_size(
        win, li_win_msg_size, event->xconfigure.width,
        event->xconfigure.height);
}

void li_win_xlib_event_close(li_win_t win) {
    li_win_send_close(win, li_win_msg_close);
}

void li_win_xlib_event(XEvent *event) {
    XPointer pointer;
    li_win_t win;
    XFindContext(
        li_win_xlib_disp, event->xany.window, li_win_xlib_context, &pointer);
    win = (li_win_t) pointer;
    switch (event->type) {
    case ClientMessage:
        if ((unsigned long) event->xclient.data.l[0]
            == li_win_xlib_wm_delete_window) {
            li_win_xlib_event_close(win);
        }
        break;
    case KeyPress:
    case KeyRelease:
        li_win_xlib_event_key(win, event, event->type == KeyPress);
        break;
    case ButtonPress:
    case ButtonRelease:
        li_win_xlib_event_button(win, event, event->type == ButtonPress);
        break;
    case MotionNotify:
        li_win_xlib_event_motion(win, event);
        break;
    case ConfigureNotify:
        li_win_xlib_event_size(win, event);
        break;
    }
}

int li_win_xlib_get_repeat(XEvent *event) {
    XEvent next_event;
    if (event->type == KeyRelease && XPending(li_win_xlib_disp)) {
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

li_input_state_t li_win_xlib_get_state(unsigned int state) {
    li_input_state_t result = 0;
    if (state & ShiftMask) {
        result |= LI_INPUT_STATE_SHIFT;
    }
    if (state & LockMask) {
        result |= LI_INPUT_STATE_CAPSLOCK;
    }
    if (state & ControlMask) {
        result |= LI_INPUT_STATE_CONTROL;
    }
    if (state & Mod1Mask) {
        result |= LI_INPUT_STATE_ALT;
    }
    if (state & Mod2Mask) {
        result |= LI_INPUT_STATE_NUMLOCK;
    }
    if (state & Mod4Mask) {
        result |= LI_INPUT_STATE_SUPER;
    }
    if (state & Button1Mask) {
        result |= LI_INPUT_STATE_LMB;
    }
    if (state & Button3Mask) {
        result |= LI_INPUT_STATE_RMB;
    }
    if (state & Button2Mask) {
        result |= LI_INPUT_STATE_MMB;
    }
    return result;
}

li_input_key_t li_win_xlib_get_keysym(KeySym sym) {
    switch (sym) {
    case XK_0:
        return LI_INPUT_KEY_0;
    case XK_1:
        return LI_INPUT_KEY_1;
    case XK_2:
        return LI_INPUT_KEY_2;
    case XK_3:
        return LI_INPUT_KEY_3;
    case XK_4:
        return LI_INPUT_KEY_4;
    case XK_5:
        return LI_INPUT_KEY_5;
    case XK_6:
        return LI_INPUT_KEY_6;
    case XK_7:
        return LI_INPUT_KEY_7;
    case XK_8:
        return LI_INPUT_KEY_8;
    case XK_9:
        return LI_INPUT_KEY_9;
    case XK_A:
        return LI_INPUT_KEY_A;
    case XK_B:
        return LI_INPUT_KEY_B;
    case XK_C:
        return LI_INPUT_KEY_C;
    case XK_D:
        return LI_INPUT_KEY_D;
    case XK_E:
        return LI_INPUT_KEY_E;
    case XK_F:
        return LI_INPUT_KEY_F;
    case XK_G:
        return LI_INPUT_KEY_G;
    case XK_H:
        return LI_INPUT_KEY_H;
    case XK_I:
        return LI_INPUT_KEY_I;
    case XK_J:
        return LI_INPUT_KEY_J;
    case XK_K:
        return LI_INPUT_KEY_K;
    case XK_L:
        return LI_INPUT_KEY_L;
    case XK_M:
        return LI_INPUT_KEY_M;
    case XK_N:
        return LI_INPUT_KEY_N;
    case XK_O:
        return LI_INPUT_KEY_O;
    case XK_P:
        return LI_INPUT_KEY_P;
    case XK_Q:
        return LI_INPUT_KEY_Q;
    case XK_R:
        return LI_INPUT_KEY_R;
    case XK_S:
        return LI_INPUT_KEY_S;
    case XK_T:
        return LI_INPUT_KEY_T;
    case XK_U:
        return LI_INPUT_KEY_U;
    case XK_V:
        return LI_INPUT_KEY_V;
    case XK_W:
        return LI_INPUT_KEY_W;
    case XK_X:
        return LI_INPUT_KEY_X;
    case XK_Y:
        return LI_INPUT_KEY_Y;
    case XK_Z:
        return LI_INPUT_KEY_Z;
    case XK_KP_0:
        return LI_INPUT_KEY_NUM0;
    case XK_KP_1:
        return LI_INPUT_KEY_NUM1;
    case XK_KP_2:
        return LI_INPUT_KEY_NUM2;
    case XK_KP_3:
        return LI_INPUT_KEY_NUM3;
    case XK_KP_4:
        return LI_INPUT_KEY_NUM4;
    case XK_KP_5:
        return LI_INPUT_KEY_NUM5;
    case XK_KP_6:
        return LI_INPUT_KEY_NUM6;
    case XK_KP_7:
        return LI_INPUT_KEY_NUM7;
    case XK_KP_8:
        return LI_INPUT_KEY_NUM8;
    case XK_KP_9:
        return LI_INPUT_KEY_NUM9;
    case XK_KP_Decimal:
        return LI_INPUT_KEY_NUMDOT;
    case XK_KP_Add:
        return LI_INPUT_KEY_NUMPLUS;
    case XK_KP_Subtract:
        return LI_INPUT_KEY_NUMMINUS;
    case XK_KP_Multiply:
        return LI_INPUT_KEY_NUMSTAR;
    case XK_KP_Divide:
        return LI_INPUT_KEY_NUMSLASH;
    case XK_KP_Enter:
        return LI_INPUT_KEY_NUMENTER;
    case XK_Num_Lock:
        return LI_INPUT_KEY_NUMLOCK;
    case XK_F1:
        return LI_INPUT_KEY_F1;
    case XK_F2:
        return LI_INPUT_KEY_F2;
    case XK_F3:
        return LI_INPUT_KEY_F3;
    case XK_F4:
        return LI_INPUT_KEY_F4;
    case XK_F5:
        return LI_INPUT_KEY_F5;
    case XK_F6:
        return LI_INPUT_KEY_F6;
    case XK_F7:
        return LI_INPUT_KEY_F7;
    case XK_F8:
        return LI_INPUT_KEY_F8;
    case XK_F9:
        return LI_INPUT_KEY_F9;
    case XK_F10:
        return LI_INPUT_KEY_F10;
    case XK_F11:
        return LI_INPUT_KEY_F11;
    case XK_F12:
        return LI_INPUT_KEY_F12;
    case XK_Print:
        return LI_INPUT_KEY_PRINTSCR;
    case XK_Scroll_Lock:
        return LI_INPUT_KEY_SCRLOCK;
    case XK_Pause:
        return LI_INPUT_KEY_PAUSE;
    case XK_Insert:
        return LI_INPUT_KEY_INSERT;
    case XK_Home:
        return LI_INPUT_KEY_HOME;
    case XK_Page_Up:
        return LI_INPUT_KEY_PAGEUP;
    case XK_Delete:
        return LI_INPUT_KEY_DELETE;
    case XK_End:
        return LI_INPUT_KEY_END;
    case XK_Page_Down:
        return LI_INPUT_KEY_PAGEDOWN;
    case XK_Up:
        return LI_INPUT_KEY_UP;
    case XK_Down:
        return LI_INPUT_KEY_DOWN;
    case XK_Left:
        return LI_INPUT_KEY_LEFT;
    case XK_Right:
        return LI_INPUT_KEY_RIGHT;
    case XK_Escape:
        return LI_INPUT_KEY_ESCAPE;
    case XK_BackSpace:
        return LI_INPUT_KEY_BSPACE;
    case XK_Tab:
        return LI_INPUT_KEY_TAB;
    case XK_Shift_L:
        return LI_INPUT_KEY_LSHIFT;
    case XK_Shift_R:
        return LI_INPUT_KEY_RSHIFT;
    case XK_Control_L:
        return LI_INPUT_KEY_LCONTROL;
    case XK_Control_R:
        return LI_INPUT_KEY_RCONTROL;
    case XK_Super_L:
        return LI_INPUT_KEY_LSUPER;
    case XK_Super_R:
        return LI_INPUT_KEY_RSUPER;
    case XK_Alt_L:
        return LI_INPUT_KEY_LALT;
    case XK_Alt_R:
        return LI_INPUT_KEY_RALT;
    case XK_Caps_Lock:
        return LI_INPUT_KEY_CAPSLOCK;
    case XK_Return:
        return LI_INPUT_KEY_ENTER;
    case XK_space:
        return LI_INPUT_KEY_SPACE;
    case XK_period:
        return LI_INPUT_KEY_DOT;
    case XK_comma:
        return LI_INPUT_KEY_COMMA;
    case XK_semicolon:
        return LI_INPUT_KEY_COLON;
    case XK_apostrophe:
        return LI_INPUT_KEY_QUOTE;
    case XK_bracketleft:
        return LI_INPUT_KEY_LBRACKET;
    case XK_bracketright:
        return LI_INPUT_KEY_RBRACKET;
    case XK_backslash:
        return LI_INPUT_KEY_BSLASH;
    case XK_minus:
        return LI_INPUT_KEY_MINUS;
    case XK_slash:
        return LI_INPUT_KEY_SLASH;
    case XK_grave:
        return LI_INPUT_KEY_TILDE;
    case XK_equal:
        return LI_INPUT_KEY_EQUAL;
    }
    return LI_INPUT_KEY_NULL;
}

li_input_key_t li_win_xlib_get_key(unsigned int key) {
    int            i;
    int            count;
    li_input_key_t result = LI_INPUT_KEY_NULL;
    KeySym        *keysym;
    keysym = XGetKeyboardMapping(li_win_xlib_disp, key, 1, &count);
    for (i = 0; i < count && keysym[i] != NoSymbol; i++) {
        result = li_win_xlib_get_keysym(keysym[i]);
        if (result != LI_INPUT_KEY_NULL) {
            break;
        }
    }
    XFree(keysym);
    return result;
}

li_input_button_t li_win_xlib_get_button(unsigned int button) {
    switch (button) {
    case Button1:
        return LI_INPUT_BUTTON_LEFT;
    case Button3:
        return LI_INPUT_BUTTON_RIGHT;
    case Button2:
        return LI_INPUT_BUTTON_MIDDLE;
    case Button4:
        return LI_INPUT_BUTTON_UP;
    case Button5:
        return LI_INPUT_BUTTON_DOWN;
    }
    return LI_INPUT_BUTTON_NULL;
}
