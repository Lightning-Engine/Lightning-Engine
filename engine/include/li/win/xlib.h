#ifndef LI_WIN_XLIB_H
#define LI_WIN_XLIB_H

#include "li/win/impl.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct li_win_xlib {
    struct li_win_base base;
    Window        window;
};

extern Display *li_win_xlib_disp;
extern Window   li_win_xlib_root;
extern XContext li_win_xlib_context;
extern Atom     li_win_xlib_atom_wm_delete_window;

int            li_win_xlib_init(void);
void           li_win_xlib_exit(void);
void           li_win_xlib_poll(void);
li_win_t       li_win_xlib_create(int width, int height);
void           li_win_xlib_destroy(li_win_t win);
int            li_win_xlib_repeat(XEvent *event);
li_win_state_t li_win_win32_state(unsigned int s);
li_win_key_t   li_win_win32_button(unsigned int button);
li_win_key_t   li_win_win32_key_impl(KeySym sym);
li_win_key_t   li_win_win32_key(unsigned int keycode);
void           li_win_xlib_event(XEvent *event);

#endif
