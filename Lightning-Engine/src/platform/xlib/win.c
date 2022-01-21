#include "li/win.h"
#include <X11/Xlib.h>

static Display *dp;
static Window root;
static Atom wm_delete_window;

int li_win_init(void) {
	dp = XOpenDisplay(NULL);
	root = XDefaultRootWindow(dp);
	wm_delete_window = XInternAtom(dp, "WM_DELETE_WINDOW", False);
	return 0;
}

void li_win_exit(void) {
	XCloseDisplay(dp);
}

void li_win_poll(void) {
	XEvent ev;
	li_win_event_t event;
	while (XPending(dp)) {
		XNextEvent(dp, &ev);
		switch (ev.type) {
			case ClientMessage:
				if (ev.xclient.data.l[0] == wm_delete_window) {
					li_win_event_t event;
					event.type = li_win_event_close;
					event.window.lu = ev.xany.window;
					li_win_cb(event);
				}
				break;
		}
	}
}

int li_win_create(li_win_t *win, int width, int height) {
	win->lu = XCreateSimpleWindow(dp, root, 0, 0, width, height, 0, 0, 0);
	XSetWMProtocols(dp, win->lu, &wm_delete_window, 1);
	return 0;
}

void li_win_destroy(li_win_t win) {
	XDestroyWindow(dp, win.lu);
}

void li_win_map(li_win_t win) {
	XMapWindow(dp, win.lu);
}
