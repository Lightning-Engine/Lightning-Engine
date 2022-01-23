#import "Cocoa/Cocoa.h"
#include "li/win.h"
#include "li/keymap.h"

static const NSUInteger LI_WINDOW_STYLE = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
static win_cb_proc_t li_win_cb;
@class LiWindowDelegate;

@interface LiWindowDelegate : NSView <NSWindowDelegate> {
@public
	bool running;
}
@end

@implementation LiWindowDelegate
- (id) initWithFrame: (NSRect) frame {
	self = [super initWithFrame:frame];

	return self;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyView
{
	return YES;
}

- (void) mouseMoved: (NSEvent *) event {
	li_event_t int_event;
	NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];

	int_event.any.type = li_event_motion_notify;
	int_event.any.window.p = [event window];
	int_event.motion.x = point.x;
	int_event.motion.y = point.y;
	li_win_cb(&event);
}

- (void) keyDown: (NSEvent *) event {
	li_event_t int_event;

	if ([event isARepeat])
		int_event.any.type = li_event_key_repeat;
	else
		int_event.any.type = li_event_key_press;
	int_event.any.window.p = [event window];
	int_event.key.key = li_win_xlat_key([event keyCode]);
	li_win_cb(&int_event);
}

- (void) keyUp: (NSEvent *) event {
	li_event_t int_event;

	int_event.any.type = li_event_key_release;
	int_event.any.window.p = [event window];
	int_event.key.key = li_win_xlat_key([event keyCode]);
	li_win_cb(&int_event);
}

- (void) windowDidMove:(NSNotification *)notification {

}
@end

static LiWindowDelegate *LI_WIN_DELEGATE = 0;

void li_win_init(win_cb_proc_t cb) {
	li_win_cb = cb;
	LI_WIN_DELEGATE = [LiWindowDelegate alloc];
	[NSApplication sharedApplication];
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
}

void li_win_exit(void) {

}

void li_win_poll(void) {
	NSEvent* event;

	event = [NSApp nextEventMatchingMask:NSEventMaskAny
		untilDate:[NSDate distantPast]
		inMode:NSDefaultRunLoopMode
		dequeue:YES];

	if (event != nil) {
		[NSApp sendEvent:event];
	}
}

li_win_t li_win_create(int width, int height) {
	li_win_t window;
	NSWindow *native_window;
	NSRect windowRect;
	LiWindowDelegate *delegate;

	windowRect = NSMakeRect(0, 0, width, height);
	native_window = [[NSWindow alloc] initWithContentRect:windowRect
								styleMask:LI_WINDOW_STYLE
								backing:NSBackingStoreBuffered
								defer:NO];   
	window.p = native_window;
	[native_window setContentView:LI_WIN_DELEGATE];
	[native_window makeFirstResponder:LI_WIN_DELEGATE];
	[native_window setDelegate:LI_WIN_DELEGATE];
	[native_window setAcceptsMouseMovedEvents:YES];
	[native_window setRestorable:NO];
	[native_window makeKeyAndOrderFront:nil];
	return window;
}

void li_win_destroy(li_win_t win) {
	NSWindow *native_window;

	native_window = win.p;
	[native_window dealloc];
}

void li_win_map(li_win_t win) {

}

li_ctx_t li_ctx_create(li_win_t win) {
	li_ctx_t context;

	context.p = 0;
	return context;
}

void li_ctx_destroy(li_ctx_t ctx) {

}

void li_ctx_make_current(li_win_t win, li_ctx_t ctx) {

}

void li_ctx_swap_buffers(li_win_t win) {

}

void *li_ctx_get_proc_addr(const char *name) {
	return 0;
}
/*
void win_cb(li_event_t *event) {
	switch (event->any.type) {
		case li_event_close:
			break;
		case li_event_key_press:
			printf("key_press { key=%d, state=%d }\n", event->key.key, event->key.state);
			break;
		case li_event_key_release:
			printf("key_release { key=%d, state=%d }\n", event->key.key, event->key.state);
			break;
		case li_event_key_repeat:
			printf("key_repeat { key=%d, state=%d }\n", event->key.key, event->key.state);
			break;
		case li_event_button_press:
			printf("button_press { x=%d, y=%d, button=%d, state=%d }\n", event->button.x, event->button.y, event->button.button, event->button.state);
			break;
		case li_event_button_release:
			printf("button_release { x=%d, y=%d, button=%d, state=%d }\n", event->button.x, event->button.y, event->button.button, event->button.state);
			break;
		case li_event_motion_notify:
			printf("motion { x=%d, y=%d, state=%d }\n", event->motion.x, event->motion.y, event->motion.state);
			break;
	}
}

int main(void) {
	li_win_init(win_cb);
	li_win_t win = li_win_create(650, 480);
	while (1)
		li_win_poll();
	return (0);
}*/
