#include "li/win.h"
#include <X11/Xlib.h>

static Display *dp;
static Window root;

void li_win_init(void) {
	dp = XOpenDisplay(NULL);
	root = XDefaultRootWindow(dp);
}

void li_win_exit(void) {
	XCloseDisplay(dp);
}

void li_win_poll(void) {
	XEvent ev;
	while (XPending(dp)) {
		XNextEvent(dp, &ev);
	}
}

li_win_t li_win_create(int width, int height) {
	li_win_t win;
	win.lu = XCreateSimpleWindow(dp, root, 0, 0, width, height, 0, 0, 0);
	return win;
}

void li_win_destroy(li_win_t win) {
	XDestroyWindow(dp, win.lu);
}

void li_win_map(li_win_t win) {
	XMapWindow(dp, win.lu);
}
