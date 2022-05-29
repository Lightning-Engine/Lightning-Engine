#ifndef LI_WIN_COCOA_H
#define LI_WIN_COCOA_H

#include "li/win/impl.h"

#import <Cocoa/Cocoa.h>

@interface LiWinCocoaWindow : NSWindow <NSWindowDelegate> {
    li_win_t win;
}
@end

@interface LiWinCocoaView : NSView {
    li_win_t win;
}
@end

struct li_win_cocoa {
    LiWinCocoaWindow *window;
    LiWinCocoaView   *view;
    li_input_state_t  state;
};

int      li_win_cocoa_init(void);
void     li_win_cocoa_exit(void);
void     li_win_cocoa_poll(void);
li_win_t li_win_cocoa_create(int width, int height);
void     li_win_cocoa_destroy(li_win_t win);

void li_win_cocoa_event_key(li_win_t win, NSEvent *event, int down);
void li_win_cocoa_event_button(
    li_win_t win, NSEvent *event, int down, li_input_button_t button);
void li_win_cocoa_event_scroll(li_win_t win, NSEvent *event);
void li_win_cocoa_event_motion(li_win_t win, NSEvent *event);
void li_win_cocoa_event_size(li_win_t win);
void li_win_cocoa_event_close(li_win_t win);

int li_win_cocoa_get_point(li_win_t win, NSEvent *event, NSPoint *point);
li_input_state_t li_win_cocoa_get_state(li_win_t win, NSUInteger state);
li_input_key_t   li_win_cocoa_get_key(unsigned short key);

#endif
