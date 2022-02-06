#include "li/win.h"
#include "li/keymap.h"
#include "li/assert.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glxext.h>

extern Display *li_xlib_display;
Display *li_xlib_display;

static Window li_xlib_root;
static int li_xlib_screen;
static Atom li_xlib_wm_delete_window;
static win_cb_proc_t li_xlib_win_cb;

static int _event_is_repeat(XEvent *ev) {
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

static void _event_handle(XEvent *ev) {
	li_event_t event;
	event.any.window.lu = ev->xany.window;
	switch (ev->type) {
		case ClientMessage:
			if ((unsigned long) ev->xclient.data.l[0] == li_xlib_wm_delete_window) {
				event.any.type = li_event_close;
				li_xlib_win_cb(&event);
			}
			break;
		case KeyPress:
		case KeyRelease:
			if (ev->type == KeyRelease)
				event.any.type = li_event_key_release;
			else if (_event_is_repeat(ev))
				event.any.type = li_event_key_repeat;
			else if (ev->type == KeyPress)
				event.any.type = li_event_key_press;
			event.key.key = li_win_xlat_key(ev->xkey.keycode);
			event.key.state = li_win_xlat_key_state(ev->xkey.state);
			li_xlib_win_cb(&event);
			break;
		case ButtonPress:
		case ButtonRelease:
			if (ev->type == ButtonPress)
				event.any.type = li_event_button_press;
			else
				event.any.type = li_event_button_release;
			event.button.x = ev->xbutton.x;
			event.button.y = ev->xbutton.y;
			event.button.button = li_win_xlat_button(ev->xbutton.button);
			event.button.state = li_win_xlat_key_state(ev->xkey.state);
			li_xlib_win_cb(&event);
			break;
		case MotionNotify:
			event.any.type = li_event_motion_notify;
			event.motion.x = ev->xmotion.x;
			event.motion.y = ev->xmotion.y;
			event.motion.state = li_win_xlat_key_state(ev->xkey.state);
			li_xlib_win_cb(&event);
			break;
		case ConfigureNotify:
			event.any.type = li_event_window_resize;
			event.resize.width = ev->xconfigure.width;
			event.resize.height = ev->xconfigure.height;
			li_xlib_win_cb(&event);
			break;
	}
}

void li_win_init(void (*cb)(li_event_t*)) {
	li_xlib_display = XOpenDisplay(NULL);
	li_assert(li_xlib_display != NULL);
	
	li_xlib_root = XDefaultRootWindow(li_xlib_display);
	li_xlib_screen = XDefaultScreen(li_xlib_display);
	li_xlib_wm_delete_window = XInternAtom(li_xlib_display, "WM_DELETE_WINDOW", False);
	li_xlib_win_cb = cb;
}

void li_win_exit(void) {
	XCloseDisplay(li_xlib_display);
}

void li_win_poll(void) {
	XEvent ev;
	while (XPending(li_xlib_display)) {
		XNextEvent(li_xlib_display, &ev);
		_event_handle(&ev);
	}
}

li_win_t li_win_create(int width, int height) {
	li_win_t win;
	win.lu = XCreateSimpleWindow(li_xlib_display, li_xlib_root, 0, 0, width, height, 0, 0, 0);
	li_assert(win.lu != 0);
	XSelectInput(li_xlib_display, win.lu, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask);
	XSetWMProtocols(li_xlib_display, win.lu, &li_xlib_wm_delete_window, 1);
	return win;
}

void li_win_destroy(li_win_t win) {
	XDestroyWindow(li_xlib_display, win.lu);
}

void li_win_map(li_win_t win) {
	XMapWindow(li_xlib_display, win.lu);
}

li_ctx_t li_ctx_create(li_win_t win, int version) {
	int visual_attribs[] = {
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER, True,
		None,
	};
	int context_attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 2,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		None,
	};

	(void) version;
	(void) win;
	int num_fbc;
	GLXFBConfig *fbc = glXChooseFBConfig(li_xlib_display, li_xlib_screen, visual_attribs, &num_fbc);
	li_assert(num_fbc > 0);
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte *) "glXCreateContextAttribsARB");
	li_assert(glXCreateContextAttribsARB != NULL);
	li_ctx_t ctx;
	ctx.p = glXCreateContextAttribsARB(li_xlib_display, fbc[0], NULL, True, context_attribs);
	li_assert(ctx.p != NULL);
	XFree(fbc);
	return ctx;
}

void li_ctx_destroy(li_win_t win, li_ctx_t ctx) {
	(void) win;
	glXDestroyContext(li_xlib_display, ctx.p);
}

void li_ctx_make_current(li_win_t win, li_ctx_t ctx) {
	glXMakeCurrent(li_xlib_display, win.lu, ctx.p);
}

void li_ctx_swap_buffers(li_win_t win, li_ctx_t ctx) {
	(void) ctx;
	glXSwapBuffers(li_xlib_display, win.lu);
}

void *li_ctx_get_proc_addr(const char *name) {
	void (*fn)(void) = glXGetProcAddress((const GLubyte *) name);
	return *(void**) &fn;
}
