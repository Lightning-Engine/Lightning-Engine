#ifndef LI_WIN_COCOA_H
#define LI_WIN_COCOA_H

#include "li/win/impl.h"

#import <Cocoa/Cocoa.h>

struct li_win_cocoa {
    struct li_win_base base;
    NSWindow          *window;
    NSView            *view;
    NSObject  *delegate;
};

int      li_win_cocoa_init(void);
void     li_win_cocoa_exit(void);
void     li_win_cocoa_poll(void);
li_win_t li_win_cocoa_create(int width, int height);
void     li_win_cocoa_destroy(li_win_t win);

li_input_state_t li_win_cocoa_xlat_state(NSUInteger state);
li_input_key_t   li_win_cocoa_xlat_key(unsigned short key);
li_input_key_t   li_win_cocoa_xlat_button(NSUInteger type);

void li_win_cocoa_event_key(li_win_t win, NSEvent *event, int down);
void li_win_cocoa_event_button(
    li_win_t win, NSEvent *event, int down, li_input_button_t button);
void li_win_cocoa_event_motion(li_win_t win, NSEvent *event);
void li_win_cocoa_event_size(li_win_t win);
void li_win_cocoa_event_close(li_win_t win);

#endif
