#include "li/win.h"
#include "li/keymap.h"
#include "li/platform/xlib.h"

Display *li_xlib_display;
static Window li_xlib_root;
static Atom li_xlib_wm_delete_window;

static int event_is_repeat(XEvent *ev) {
	XEvent ev2;
	if (XPending(li_xlib_display)) {
		XPeekEvent(li_xlib_display, &ev2);
		if (ev2.type == KeyPress && ev->xkey.time == ev2.xkey.time && ev->xkey.keycode == ev2.xkey.keycode) {
			XNextEvent(li_xlib_display, &ev2);
			return 1;
		}
	}
	return 0;
}

static void event_handle(XEvent *ev) {
	li_event_t event;
	event.window.lu = ev->xany.window;
	switch (ev->type) {
		case ClientMessage:
			if (ev->xclient.data.l[0] == li_xlib_wm_delete_window) {
				event.type = li_event_close;
				li_win_cb(&event);
			}
			break;
		case KeyPress:
		case KeyRelease:
			if (ev->type == KeyRelease)
				event.type = li_event_key_release;
			else if (event_is_repeat(ev))
				event.type = li_event_key_repeat;
			else if (ev->type == KeyPress)
				event.type = li_event_key_press;
			event.data.key.key = li_win_xlat_key(ev->xkey.keycode);
			li_win_cb(&event);
			break;
		case ButtonPress:
		case ButtonRelease:
			if (ev->type == ButtonPress)
				event.type = li_event_button_press;
			else
				event.type = li_event_button_release;
			event.data.button.x = ev->xbutton.x;
			event.data.button.y = ev->xbutton.y;
			event.data.button.button = li_win_xlat_button(ev->xbutton.button);
			li_win_cb(&event);
			break;
		case MotionNotify:
			event.type = li_event_motion_notify;
			event.data.motion.x = ev->xmotion.x;
			event.data.motion.y = ev->xmotion.y;
			li_win_cb(&event);
			break;
	}
}

int li_win_init(void) {
	li_xlib_display = XOpenDisplay(NULL);
	if (li_xlib_display == NULL)
		return -1;
	
	li_xlib_root = XDefaultRootWindow(li_xlib_display);
	li_xlib_wm_delete_window = XInternAtom(li_xlib_display, "WM_DELETE_WINDOW", False);
	return 0;
}

void li_win_exit(void) {
	XCloseDisplay(li_xlib_display);
}

void li_win_poll(void) {
	XEvent ev;
	while (XPending(li_xlib_display)) {
		XNextEvent(li_xlib_display, &ev);
		event_handle(&ev);
	}
}

int li_win_create(li_win_t *win, int width, int height) {
	win->lu = XCreateSimpleWindow(li_xlib_display, li_xlib_root, 0, 0, width, height, 0, 0, 0);
	XSelectInput(li_xlib_display, win->lu, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask);
	XSetWMProtocols(li_xlib_display, win->lu, &li_xlib_wm_delete_window, 1);
	return 0;
}

void li_win_destroy(li_win_t win) {
	XDestroyWindow(li_xlib_display, win.lu);
}

void li_win_map(li_win_t win) {
	XMapWindow(li_xlib_display, win.lu);
}
