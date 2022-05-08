#ifndef LI_WIN_XLIB_H
#define LI_WIN_XLIB_H

#include "li/win/impl.h"

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

struct li_win_xlib {
    Window window;
};

extern Display *li_win_xlib_disp;
extern Window   li_win_xlib_root;
extern XContext li_win_xlib_context;
extern Atom     li_win_xlib_wm_delete_window;

int      li_win_xlib_init(void);
void     li_win_xlib_exit(void);
void     li_win_xlib_poll(void);
li_win_t li_win_xlib_create(int width, int height);
void     li_win_xlib_destroy(li_win_t win);

void li_win_xlib_event_key(li_win_t win, XEvent *event, int down);
void li_win_xlib_event_button(li_win_t win, XEvent *event, int down);
void li_win_xlib_event_motion(li_win_t win, XEvent *event);
void li_win_xlib_event_size(li_win_t win, XEvent *event);
void li_win_xlib_event_close(li_win_t win);
void li_win_xlib_event(XEvent *event);

int               li_win_xlib_get_repeat(XEvent *event);
li_input_state_t  li_win_xlib_get_state(unsigned int state);
li_input_key_t    li_win_xlib_get_keysym(KeySym keysym);
li_input_key_t    li_win_xlib_get_key(unsigned int key);
li_input_button_t li_win_xlib_get_button(unsigned int button);

#endif
