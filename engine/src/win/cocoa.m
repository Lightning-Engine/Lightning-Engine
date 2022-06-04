#include "li/win/cocoa.h"

#include "li/std.h"

const struct li_win_impl li_win_cocoa_impl = {
    li_win_cocoa_init, li_win_cocoa_exit, li_win_cocoa_poll,
    li_win_cocoa_create, li_win_cocoa_destroy
};

@implementation LiWinCocoaWindow
- (id)init:(li_win_t)_win width:(int)width height:(int)height {
    self = [super
        initWithContentRect:NSMakeRect(0, 0, width, height)
                  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable
                            | NSWindowStyleMaskResizable
                    backing:NSBackingStoreBuffered
                      defer:NO];
    if (self) {
        win = _win;
    }
    return self;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyWindow {
    return YES;
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    (void) sender;
    li_win_cocoa_event_close(win);
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification {
    (void) notification;
    li_win_cocoa_event_size(win);
}
@end

@implementation LiWinCocoaView
- (id)init:(li_win_t)_win {
    self = [super init];
    if (self) {
        win = _win;
    }
    return self;
}

- (void)keyDown:(NSEvent *)event {
    li_win_cocoa_event_key(win, event, 1);
}

- (void)keyUp:(NSEvent *)event {
    li_win_cocoa_event_key(win, event, 0);
}

- (void)mouseDown:(NSEvent *)event {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    win_cocoa->state |= LI_INPUT_STATE_LMB;
    li_win_cocoa_event_button(win, event, 1, LI_INPUT_BUTTON_LEFT);
}

- (void)mouseUp:(NSEvent *)event {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    win_cocoa->state &= ~LI_INPUT_STATE_LMB;
    li_win_cocoa_event_button(win, event, 0, LI_INPUT_BUTTON_LEFT);
}

- (void)rightMouseDown:(NSEvent *)event {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    win_cocoa->state |= LI_INPUT_STATE_RMB;
    li_win_cocoa_event_button(win, event, 1, LI_INPUT_BUTTON_RIGHT);
}

- (void)rightMouseUp:(NSEvent *)event {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    win_cocoa->state &= ~LI_INPUT_STATE_RMB;
    li_win_cocoa_event_button(win, event, 0, LI_INPUT_BUTTON_RIGHT);
}

- (void)otherMouseDown:(NSEvent *)event {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    win_cocoa->state |= LI_INPUT_STATE_MMB;
    li_win_cocoa_event_button(win, event, 1, LI_INPUT_BUTTON_MIDDLE);
}

- (void)otherMouseUp:(NSEvent *)event {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    win_cocoa->state &= ~LI_INPUT_STATE_MMB;
    li_win_cocoa_event_button(win, event, 0, LI_INPUT_BUTTON_MIDDLE);
}

- (void)scrollWheel:(NSEvent *)event {
    li_win_cocoa_event_scroll(win, event);
}

- (void)mouseMoved:(NSEvent *)event {
    li_win_cocoa_event_motion(win, event);
}

- (void)mouseDragged:(NSEvent *)event {
    li_win_cocoa_event_motion(win, event);
}

- (void)rightMouseDragged:(NSEvent *)event {
    li_win_cocoa_event_motion(win, event);
}

- (void)otherMouseDragged:(NSEvent *)event {
    li_win_cocoa_event_motion(win, event);
}
@end

int li_win_cocoa_init(void) {
    li_win_impl = &li_win_cocoa_impl;
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    return 0;
}

void li_win_cocoa_exit(void) { }

void li_win_cocoa_poll(void) {
    NSEvent *event;
    while (1) {
        event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                   untilDate:[NSDate distantPast]
                                      inMode:NSDefaultRunLoopMode
                                     dequeue:YES];
        if (event == nil) {
            break;
        }
        [NSApp sendEvent:event];
    }
}

li_win_t li_win_cocoa_create(int width, int height) {
    struct li_win_cocoa *win_cocoa;
    win_cocoa = li_std_malloc(sizeof *win_cocoa);
    if (win_cocoa != NULL) {
        win_cocoa->window = [[LiWinCocoaWindow alloc] init:(li_win_t) win_cocoa
                                                     width:width
                                                    height:height];
        win_cocoa->view   = [[LiWinCocoaView alloc] init:(li_win_t) win_cocoa];
        win_cocoa->state  = 0;
        if (win_cocoa->window != nil && win_cocoa->view != nil) {
            [win_cocoa->window setContentView:win_cocoa->view];
            [win_cocoa->window setDelegate:win_cocoa->window];
            [win_cocoa->window setAcceptsMouseMovedEvents:YES];
            [win_cocoa->window makeFirstResponder:win_cocoa->view];
            [win_cocoa->window makeKeyAndOrderFront:nil];
            return (li_win_t) win_cocoa;
        }
        if (win_cocoa->window != nil) {
            [win_cocoa->window release];
        }
        if (win_cocoa->view != nil) {
            [win_cocoa->view release];
        }
        li_std_free(win_cocoa);
    }
    return NULL;
}

void li_win_cocoa_destroy(li_win_t win) {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    [win_cocoa->window close];
    [win_cocoa->window release];
    [win_cocoa->view release];
    li_std_free(win_cocoa);
}

void li_win_cocoa_event_key(li_win_t win, NSEvent *event, int down) {
    li_win_send_key(
        win,
        [event isARepeat] ? li_win_msg_key_repeat
        : down            ? li_win_msg_key_down
                          : li_win_msg_key_up,
        li_win_cocoa_get_state(win, [event modifierFlags]),
        li_win_cocoa_get_key([event keyCode]));
}

void li_win_cocoa_event_button(
    li_win_t win, NSEvent *event, int down, li_input_button_t button) {
    NSPoint location;
    if (li_win_cocoa_get_point(win, event, &location)) {
        li_win_send_button(
            win, down ? li_win_msg_button_down : li_win_msg_button_up,
            li_win_cocoa_get_state(win, [event modifierFlags]), location.x,
            location.y, button);
    }
}

void li_win_cocoa_event_scroll(li_win_t win, NSEvent *event) {
    li_input_state_t  state;
    li_input_button_t button;
    NSPoint           location;
    state  = li_win_cocoa_get_state(win, [event modifierFlags]);
    button = [event deltaY] > 0 ? LI_INPUT_BUTTON_UP : LI_INPUT_BUTTON_DOWN;
    if (li_win_cocoa_get_point(win, event, &location)) {
        li_win_send_button(
            win, li_win_msg_button_down, state, location.x, location.y, button);
        li_win_send_button(
            win, li_win_msg_button_up, state, location.x, location.y, button);
    }
}

void li_win_cocoa_event_motion(li_win_t win, NSEvent *event) {
    NSPoint location;
    if (li_win_cocoa_get_point(win, event, &location)) {
        li_win_send_motion(
            win, li_win_msg_motion,
            li_win_cocoa_get_state(win, [event modifierFlags]), location.x,
            location.y);
    }
}

void li_win_cocoa_event_size(li_win_t win) {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    NSRect               frame;
    frame = [win_cocoa->view frame];
    li_win_send_size(win, li_win_msg_size, frame.size.width, frame.size.height);
}

void li_win_cocoa_event_close(li_win_t win) {
    li_win_send_close(win, li_win_msg_close);
}

int li_win_cocoa_get_point(li_win_t win, NSEvent *event, NSPoint *point) {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    NSRect               frame;
    frame    = [win_cocoa->view frame];
    *point   = [event locationInWindow];
    point->x = point->x - frame.origin.x;
    point->y = frame.origin.y - point->y + frame.size.height;
    return point->x >= 0 && point->y >= 0 && point->x < frame.size.width
        && point->y < frame.size.height;
}

li_input_state_t li_win_cocoa_get_state(li_win_t win, NSUInteger state) {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    li_input_state_t     result    = win_cocoa->state;
    if (state & NSShiftKeyMask) {
        result |= LI_INPUT_STATE_SHIFT;
    }
    if (state & NSAlphaShiftKeyMask) {
        result |= LI_INPUT_STATE_CAPSLOCK;
    }
    if (state & NSControlKeyMask) {
        result |= LI_INPUT_STATE_CONTROL;
    }
    if (state & NSAlternateKeyMask) {
        result |= LI_INPUT_STATE_ALT;
    }
    if (state & NSCommandKeyMask) {
        result |= LI_INPUT_STATE_SUPER;
    }
    return (result);
}

li_input_key_t li_win_cocoa_get_key(unsigned short key) {
    switch (key) {
    case 29:
        return LI_INPUT_KEY_0;
    case 18:
        return LI_INPUT_KEY_1;
    case 19:
        return LI_INPUT_KEY_2;
    case 20:
        return LI_INPUT_KEY_3;
    case 21:
        return LI_INPUT_KEY_4;
    case 22:
        return LI_INPUT_KEY_5;
    case 23:
        return LI_INPUT_KEY_6;
    case 26:
        return LI_INPUT_KEY_7;
    case 28:
        return LI_INPUT_KEY_8;
    case 25:
        return LI_INPUT_KEY_9;
    case 0:
        return LI_INPUT_KEY_A;
    case 11:
        return LI_INPUT_KEY_B;
    case 8:
        return LI_INPUT_KEY_C;
    case 2:
        return LI_INPUT_KEY_D;
    case 14:
        return LI_INPUT_KEY_E;
    case 3:
        return LI_INPUT_KEY_F;
    case 5:
        return LI_INPUT_KEY_G;
    case 4:
        return LI_INPUT_KEY_H;
    case 34:
        return LI_INPUT_KEY_I;
    case 38:
        return LI_INPUT_KEY_J;
    case 40:
        return LI_INPUT_KEY_K;
    case 37:
        return LI_INPUT_KEY_L;
    case 46:
        return LI_INPUT_KEY_M;
    case 45:
        return LI_INPUT_KEY_N;
    case 31:
        return LI_INPUT_KEY_O;
    case 35:
        return LI_INPUT_KEY_P;
    case 12:
        return LI_INPUT_KEY_Q;
    case 15:
        return LI_INPUT_KEY_R;
    case 1:
        return LI_INPUT_KEY_S;
    case 17:
        return LI_INPUT_KEY_T;
    case 32:
        return LI_INPUT_KEY_U;
    case 9:
        return LI_INPUT_KEY_V;
    case 13:
        return LI_INPUT_KEY_W;
    case 7:
        return LI_INPUT_KEY_X;
    case 16:
        return LI_INPUT_KEY_Y;
    case 6:
        return LI_INPUT_KEY_Z;
    case 82:
        return LI_INPUT_KEY_NUM0;
    case 83:
        return LI_INPUT_KEY_NUM1;
    case 84:
        return LI_INPUT_KEY_NUM2;
    case 85:
        return LI_INPUT_KEY_NUM3;
    case 86:
        return LI_INPUT_KEY_NUM4;
    case 87:
        return LI_INPUT_KEY_NUM5;
    case 88:
        return LI_INPUT_KEY_NUM6;
    case 89:
        return LI_INPUT_KEY_NUM7;
    case 91:
        return LI_INPUT_KEY_NUM8;
    case 92:
        return LI_INPUT_KEY_NUM9;
    case 65:
        return LI_INPUT_KEY_NUMDOT;
    case 69:
        return LI_INPUT_KEY_NUMPLUS;
    case 78:
        return LI_INPUT_KEY_NUMMINUS;
    case 67:
        return LI_INPUT_KEY_NUMSTAR;
    case 75:
        return LI_INPUT_KEY_NUMSLASH;
    case 76:
        return LI_INPUT_KEY_NUMENTER;
    case 71:
        return LI_INPUT_KEY_NUMLOCK;
    case 122:
        return LI_INPUT_KEY_F1;
    case 120:
        return LI_INPUT_KEY_F2;
    case 99:
        return LI_INPUT_KEY_F3;
    case 118:
        return LI_INPUT_KEY_F4;
    case 96:
        return LI_INPUT_KEY_F5;
    case 97:
        return LI_INPUT_KEY_F6;
    case 98:
        return LI_INPUT_KEY_F7;
    case 100:
        return LI_INPUT_KEY_F8;
    case 101:
        return LI_INPUT_KEY_F9;
    case 109:
        return LI_INPUT_KEY_F10;
    case 103:
        return LI_INPUT_KEY_F11;
    case 111:
        return LI_INPUT_KEY_F12;
    case 105:
        return LI_INPUT_KEY_PRINTSCR;
    case 114:
        return LI_INPUT_KEY_INSERT;
    case 115:
        return LI_INPUT_KEY_HOME;
    case 116:
        return LI_INPUT_KEY_PAGEUP;
    case 117:
        return LI_INPUT_KEY_DELETE;
    case 119:
        return LI_INPUT_KEY_END;
    case 121:
        return LI_INPUT_KEY_PAGEDOWN;
    case 126:
        return LI_INPUT_KEY_UP;
    case 125:
        return LI_INPUT_KEY_DOWN;
    case 123:
        return LI_INPUT_KEY_LEFT;
    case 124:
        return LI_INPUT_KEY_RIGHT;
    case 53:
        return LI_INPUT_KEY_ESCAPE;
    case 51:
        return LI_INPUT_KEY_BSPACE;
    case 48:
        return LI_INPUT_KEY_TAB;
    case 56:
        return LI_INPUT_KEY_LSHIFT;
    case 60:
        return LI_INPUT_KEY_RSHIFT;
    case 59:
        return LI_INPUT_KEY_LCONTROL;
    case 62:
        return LI_INPUT_KEY_RCONTROL;
    case 55:
        return LI_INPUT_KEY_LSUPER;
    case 54:
        return LI_INPUT_KEY_RSUPER;
    case 58:
        return LI_INPUT_KEY_LALT;
    case 61:
        return LI_INPUT_KEY_RALT;
    case 57:
        return LI_INPUT_KEY_CAPSLOCK;
    case 36:
        return LI_INPUT_KEY_ENTER;
    case 49:
        return LI_INPUT_KEY_SPACE;
    case 47:
        return LI_INPUT_KEY_DOT;
    case 43:
        return LI_INPUT_KEY_COMMA;
    case 41:
        return LI_INPUT_KEY_COLON;
    case 39:
        return LI_INPUT_KEY_QUOTE;
    case 33:
        return LI_INPUT_KEY_LBRACKET;
    case 30:
        return LI_INPUT_KEY_RBRACKET;
    case 42:
        return LI_INPUT_KEY_BSLASH;
    case 27:
        return LI_INPUT_KEY_MINUS;
    case 44:
        return LI_INPUT_KEY_SLASH;
    case 50:
        return LI_INPUT_KEY_TILDE;
    case 24:
        return LI_INPUT_KEY_EQUAL;
    }
    return LI_INPUT_KEY_NULL;
}
