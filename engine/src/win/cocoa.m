#include "li/win/cocoa.h"

#import "Cocoa/Cocoa.h"
#include "li/std.h"

@interface LiWinCocoaDelegate : NSObject <NSWindowDelegate> {
    struct li_win_cocoa *win;
}
@end

@interface LiWinCocoaView : NSView {
    struct li_win_cocoa *win;
    li_win_state_t       state;
}
@end

@implementation LiWinCocoaDelegate
- (BOOL)windowShouldClose:(NSWindow *)sender {
    li_win_win = win;
    li_win_fun(li_win_msg_close);
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification {
    li_win_win         = win;
    li_win_win->width  = [win->view frame].size.width;
    li_win_win->height = [win->view frame].size.height;
    li_win_fun(li_win_msg_resize);
}
@end

@implementation LiWinCocoaView
- (void)keyDown:(NSEvent *)event {
    li_win_win        = win;
    li_win_key        = [event keyCode];
    li_win_win->state = state | li_win_cocoa_state([event modifierFlags]);
    if ([event isARepeat]) {
        li_win_fun(li_win_msg_keyrep);
    } else {
        li_win_fun(li_win_msg_keydown);
    }
}

- (void)keyUp:(NSEvent *)event {
    li_win_win        = win;
    li_win_win->key   = [event keyCode];
    li_win_win->state = state | li_win_cocoa_state([event modifierFlags]);
    li_win_fun(li_win_msg_keyup);
}

- (void)mouseDown:(NSEvent *)event {
    state |= LI_KEY_STATE_LMOUSE;
    li_win_cocoa_mousedown(win, state, event);
}

- (void)mouseUp:(NSEvent *)event {
    state &= ~LI_KEY_STATE_LMOUSE;
    li_win_cocoa_mouseup(win, state, event);
}

- (void)rightMouseDown:(NSEvent *)event {
    state |= LI_KEY_STATE_RMOUSE;
    li_win_cocoa_mousedown(win, state, event);
}

- (void)rightMouseUp:(NSEvent *)event {
    state &= ~LI_KEY_STATE_RMOUSE;
    li_win_cocoa_mouseup(win, state, event);
}

- (void)otherMouseDown:(NSEvent *)event {
    state |= LI_KEY_STATE_MMOUSE;
    li_win_cocoa_mousedown(win, state, event);
}

- (void)otherMouseUp:(NSEvent *)event {
    state &= ~LI_KEY_STATE_MMOUSE;
    li_win_cocoa_mouseup(win, state, event);
}

- (void)mouseMoved:(NSEvent *)event {
    li_win_cocoa_mousemove(win, event);
}

- (void)mouseDragged:(NSEvent *)event {
    li_win_cocoa_mousemove(win, event);
}

- (void)rightMouseDragged:(NSEvent *)event {
    li_win_cocoa_mousemove(win, event);
}

- (void)otherMouseDragged:(NSEvent *)event {
    li_win_cocoa_mousemove(win, event);
}
@end

const struct li_win_impl li_win_cocoa_impl = { li_win_cocoa_exit,
                                               li_win_cocoa_poll,
                                               li_win_cocoa_create,
                                               li_win_cocoa_destroy };

int li_win_cocoa_init(void) {
    li_win_impl = &li_win_cocoa_impl;
    [NSApplication sharedApplication];
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
        win_cocoa->window   = [[NSWindow alloc]
            initWithContentRect:NSMakeRect(0, 0, width, height)
                      styleMask:NSWindowStyleMaskTitled
                                | NSWindowStyleMaskClosable
                                | NSWindowStyleMaskResizable
                        backing:NSBackingStoreBuffered
                          defer:NO];
        win_cocoa->view     = [LiWinCocoaView new];
        win_cocoa->delegate = [LiWinCocoaDelegate new];
        if (win_cocoa->window != nil && win_cocoa->view != nil
            && win_cocoa->delegate != nil) {
            [win_cocoa->view self].win     = win_cocoa;
            [win_cocoa->view self].state   = 0;
            [win_cocoa->delegate self].win = win_cocoa;
            [win_cocoa->window setContentView:win_cocoa->view];
            [win_cocoa->window makeFirstResponder:win_cocoa->view];
            [win_cocoa->window setDelegate:win_cocoa->delegate];
            [win_cocoa->window setAcceptsMouseMovements:YES];
            [win_cocoa->window makeKeyAndOrderFront:nil];
            return (li_win_t) win_cocoa;
        }
        li_std_free(win_cocoa);
    }
    return NULL;
}

void li_win_cocoa_destroy(li_win_t win) {
    struct li_win_cocoa *win_cocoa = (struct li_win_cocoa *) win;
    [win_cocoa->window close];
    li_std_free(win_cocoa);
}

li_win_state_t li_win_cocoa_state(NSUInteger flags) {
    li_win_state_t state = 0;
    if (flags & NSShiftKeyMask) {
        state |= LI_KEY_STATE_SHIFT;
    }
    if (flags & NSAlphaShiftKeyMask) {
        state |= LI_KEY_STATE_CAPS;
    }
    if (flags & NSControlKeyMask) {
        state |= LI_KEY_STATE_CTRL;
    }
    if (flags & NSAlternateKeyMask) {
        state |= LI_KEY_STATE_ALT;
    }
    if (flags & NSCommandKeyMask) {
        state |= LI_KEY_STATE_SUPER;
    }
    return (state);
}

li_win_key_t li_win_cocoa_button(NSUInteger type) {
    switch (type) {
    case NSEventTypeLeftMouseDown:
    case NSEventTypeLeftMouseUp:
        return LI_KEY_CODE_LMOUSE;
    case NSEventTypeRightMouseDown:
    case NSEventTypeRightMouseUp:
        return LI_KEY_CODE_RMOUSE;
    case NSEventTypeOtherMouseDown:
    case NSEventTypeOtherMouseUp:
        return LI_KEY_CODE_MMOUSE;
    }
    return LI_KEY_CODE_NULL;
}

li_win_key_t li_win_cocoa_key(unsigned short keycode) {
    switch (keycode) {
    case 29:
        return LI_KEY_CODE_0;
    case 18:
        return LI_KEY_CODE_1;
    case 19:
        return LI_KEY_CODE_2;
    case 20:
        return LI_KEY_CODE_3;
    case 21:
        return LI_KEY_CODE_4;
    case 22:
        return LI_KEY_CODE_5;
    case 23:
        return LI_KEY_CODE_6;
    case 26:
        return LI_KEY_CODE_7;
    case 28:
        return LI_KEY_CODE_8;
    case 25:
        return LI_KEY_CODE_9;
    case 0:
        return LI_KEY_CODE_A;
    case 11:
        return LI_KEY_CODE_B;
    case 8:
        return LI_KEY_CODE_C;
    case 2:
        return LI_KEY_CODE_D;
    case 14:
        return LI_KEY_CODE_E;
    case 3:
        return LI_KEY_CODE_F;
    case 5:
        return LI_KEY_CODE_G;
    case 4:
        return LI_KEY_CODE_H;
    case 34:
        return LI_KEY_CODE_I;
    case 38:
        return LI_KEY_CODE_J;
    case 40:
        return LI_KEY_CODE_K;
    case 37:
        return LI_KEY_CODE_L;
    case 46:
        return LI_KEY_CODE_M;
    case 45:
        return LI_KEY_CODE_N;
    case 31:
        return LI_KEY_CODE_O;
    case 35:
        return LI_KEY_CODE_P;
    case 12:
        return LI_KEY_CODE_Q;
    case 15:
        return LI_KEY_CODE_R;
    case 1:
        return LI_KEY_CODE_S;
    case 17:
        return LI_KEY_CODE_T;
    case 32:
        return LI_KEY_CODE_U;
    case 9:
        return LI_KEY_CODE_V;
    case 13:
        return LI_KEY_CODE_W;
    case 7:
        return LI_KEY_CODE_X;
    case 16:
        return LI_KEY_CODE_Y;
    case 6:
        return LI_KEY_CODE_Z;
    case 82:
        return LI_KEY_CODE_NUM0;
    case 83:
        return LI_KEY_CODE_NUM1;
    case 84:
        return LI_KEY_CODE_NUM2;
    case 85:
        return LI_KEY_CODE_NUM3;
    case 86:
        return LI_KEY_CODE_NUM4;
    case 87:
        return LI_KEY_CODE_NUM5;
    case 88:
        return LI_KEY_CODE_NUM6;
    case 89:
        return LI_KEY_CODE_NUM7;
    case 91:
        return LI_KEY_CODE_NUM8;
    case 92:
        return LI_KEY_CODE_NUM9;
    case 65:
        return LI_KEY_CODE_NUMDOT;
    case 69:
        return LI_KEY_CODE_NUMPLUS;
    case 78:
        return LI_KEY_CODE_NUMMINUS;
    case 67:
        return LI_KEY_CODE_NUMSTAR;
    case 75:
        return LI_KEY_CODE_NUMSLASH;
    case 76:
        return LI_KEY_CODE_NUMENTER;
    case 71:
        return LI_KEY_CODE_NUMLOCK;
    case 122:
        return LI_KEY_CODE_F1;
    case 120:
        return LI_KEY_CODE_F2;
    case 99:
        return LI_KEY_CODE_F3;
    case 118:
        return LI_KEY_CODE_F4;
    case 96:
        return LI_KEY_CODE_F5;
    case 97:
        return LI_KEY_CODE_F6;
    case 98:
        return LI_KEY_CODE_F7;
    case 100:
        return LI_KEY_CODE_F8;
    case 101:
        return LI_KEY_CODE_F9;
    case 109:
        return LI_KEY_CODE_F10;
    case 103:
        return LI_KEY_CODE_F11;
    case 111:
        return LI_KEY_CODE_F12;
    case 105:
        return LI_KEY_CODE_PRINTSCR;
    case 114:
        return LI_KEY_CODE_INSERT;
    case 115:
        return LI_KEY_CODE_HOME;
    case 116:
        return LI_KEY_CODE_PAGEUP;
    case 117:
        return LI_KEY_CODE_DELETE;
    case 119:
        return LI_KEY_CODE_END;
    case 121:
        return LI_KEY_CODE_PAGEDOWN;
    case 126:
        return LI_KEY_CODE_UP;
    case 125:
        return LI_KEY_CODE_DOWN;
    case 123:
        return LI_KEY_CODE_LEFT;
    case 124:
        return LI_KEY_CODE_RIGHT;
    case 53:
        return LI_KEY_CODE_ESCAPE;
    case 51:
        return LI_KEY_CODE_BSPACE;
    case 48:
        return LI_KEY_CODE_TAB;
    case 56:
        return LI_KEY_CODE_LSHIFT;
    case 60:
        return LI_KEY_CODE_RSHIFT;
    case 59:
        return LI_KEY_CODE_LCONTROL;
    case 62:
        return LI_KEY_CODE_RCONTROL;
    case 55:
        return LI_KEY_CODE_LSUPER;
    case 54:
        return LI_KEY_CODE_RSUPER;
    case 58:
        return LI_KEY_CODE_LALT;
    case 61:
        return LI_KEY_CODE_RALT;
    case 57:
        return LI_KEY_CODE_CAPSLOCK;
    case 36:
        return LI_KEY_CODE_ENTER;
    case 49:
        return LI_KEY_CODE_SPACE;
    case 47:
        return LI_KEY_CODE_DOT;
    case 43:
        return LI_KEY_CODE_COMMA;
    case 41:
        return LI_KEY_CODE_COLON;
    case 39:
        return LI_KEY_CODE_QUOTE;
    case 33:
        return LI_KEY_CODE_LBRACKET;
    case 30:
        return LI_KEY_CODE_RBRACKET;
    case 42:
        return LI_KEY_CODE_BSLASH;
    case 27:
        return LI_KEY_CODE_MINUS;
    case 44:
        return LI_KEY_CODE_SLASH;
    case 50:
        return LI_KEY_CODE_TILDE;
    case 24:
        return LI_KEY_CODE_EQUAL;
    }
    return LI_KEY_CODE_NULL;
}

void li_win_cocoa_mousedown(
    struct li_win_cocoa *win, li_win_state_t state, NSEvent *event) {
    li_win_win         = (li_win_t) win;
    li_win_key         = li_win_cocoa_button([event type]);
    li_win_win->state  = state | li_win_cocoa_state([event modifierFlags]);
    li_win_win->mousex = [event mouseLocation].x;
    li_win_win->mousey = [event mouseLocation].y;
    li_win_fun(li_win_msg_mousedown);
}

void li_win_cocoa_mouseup(
    struct li_win_cocoa *win, li_win_state_t state, NSEvent *event) {
    li_win_win         = (li_win_t) win;
    li_win_key         = li_win_cocoa_button([event type]);
    li_win_win->state  = state | li_win_cocoa_state([event modifierFlags]);
    li_win_win->mousex = [event mouseLocation].x;
    li_win_win->mousey = [event mouseLocation].y;
    li_win_fun(li_win_msg_mouseup);
}

void li_win_cocoa_mousemove(
    struct li_win_cocoa *win, li_win_state_t state, NSEvent *event) {
    li_win_win         = (li_win_t) win;
    li_win_win->state  = state | li_win_cocoa_state([event modifierFlags]);
    li_win_win->mousex = [event mouseLocation].x;
    li_win_win->mousey = [event mouseLocation].y;
    li_win_fun(li_win_msg_mousemove);
}
