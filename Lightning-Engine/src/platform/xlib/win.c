#include "li/win.h"
#include <X11/Xlib.h>

static Display *dp;
static Window root;
static Atom wm_delete_window;

static void handle_event(XEvent ev) {
	li_event_t event;
	switch (ev.type) {
		case ClientMessage:
			if (ev.xclient.data.l[0] == wm_delete_window) {
				event.type = li_event_close;
				event.window.lu = ev.xany.window;
				li_win_cb(event);
			}
			break;
		case KeyPress:
			event.type = li_event_key_press;
			event.window.lu = ev.xkey.window;
			event.data.key.key = ev.xkey.keycode;
			li_win_cb(event);
			break;
		case KeyRelease:
			event.type = li_event_key_release;
			event.window.lu = ev.xkey.window;
			event.data.key.key = ev.xkey.keycode;
			li_win_cb(event);
			break;
		case ButtonPress:
			event.type = li_event_button_press;
			event.window.lu = ev.xbutton.window;
			event.data.button.x = ev.xbutton.x;
			event.data.button.y = ev.xbutton.y;
			event.data.button.button = ev.xbutton.button;
			li_win_cb(event);
			break;
		case ButtonRelease:
			event.type = li_event_button_release;
			event.window.lu = ev.xbutton.window;
			event.data.button.x = ev.xbutton.x;
			event.data.button.y = ev.xbutton.y;
			event.data.button.button = ev.xbutton.button;
			li_win_cb(event);
			break;
		case MotionNotify:
			event.type = li_event_motion_notify;
			event.window.lu = ev.xmotion.window;
			event.data.motion.x = ev.xmotion.x;
			event.data.motion.y = ev.xmotion.y;
			li_win_cb(event);
			break;
	}
}

int li_win_init(void) {
	dp = XOpenDisplay(NULL);
	if (dp == NULL)
		return -1;
	
	root = XDefaultRootWindow(dp);
	wm_delete_window = XInternAtom(dp, "WM_DELETE_WINDOW", False);
	return 0;
}

void li_win_exit(void) {
	XCloseDisplay(dp);
}

void li_win_poll(void) {
	XEvent ev;
	while (XPending(dp)) {
		XNextEvent(dp, &ev);
		handle_event(ev);
	}
}

int li_win_create(li_win_t *win, int width, int height) {
	win->lu = XCreateSimpleWindow(dp, root, 0, 0, width, height, 0, 0, 0);
	XSelectInput(dp, win->lu, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask);
	XSetWMProtocols(dp, win->lu, &wm_delete_window, 1);
	return 0;
}

void li_win_destroy(li_win_t win) {
	XDestroyWindow(dp, win.lu);
}

void li_win_map(li_win_t win) {
	XMapWindow(dp, win.lu);
}
