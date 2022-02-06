#import "Cocoa/Cocoa.h"
#import "OpenGL/OpenGL.h"
#import <mach-o/dyld.h>
#import <stdlib.h>
#import <string.h>
#include "li/win.h"
#include "li/keymap.h"
#include "li/assert.h"
#include <OpenGL/gl.h>
#include <stdio.h>

static const NSUInteger li_cocoa_win_style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
static win_cb_proc_t li_cocoa_win_cb;
static CFBundleRef li_cocoa_framework;

@interface LiCocoaWindow : NSWindow {
}
@property (readwrite) void *user_pointer;
@end

@interface LiCocoaWindowDelegate : NSObject <NSWindowDelegate> {
	LiCocoaWindow *window;
}
- (instancetype) _init_with_window:(LiCocoaWindow*)init_window;
@end

@interface LiView : NSView {
@public
	LiCocoaWindow *window;
}
- (instancetype) _init_with_window:(LiCocoaWindow*)init_window;
- (void) _handle_button_event:(NSEvent*)event button:(int)eventButton pressed:(int) isPressed;
- (void) _handle_mouse_move_event:(NSEvent*)event;
+ (int) _get_mouse_state;
@end

@implementation LiCocoaWindow
@synthesize user_pointer;

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyView {
	return YES;
}
@end

@implementation LiCocoaWindowDelegate
- (instancetype) _init_with_window:(LiCocoaWindow*)init_window {
	self = [super init];
	window = init_window;
	return self;
}

- (void) windowDidResize:(NSNotification *) notification {
	li_event_t int_event;
	NSSize size = [[window contentView] frame].size;

	int_event.any.type = li_event_window_resize;
	int_event.any.window.p = window;
	int_event.resize.width = (int) size.width;
	int_event.resize.height = (int) size.height;
	li_cocoa_win_cb(&int_event);
}

- (BOOL) windowShouldClose:(NSNotification *)notification {
	li_event_t int_event;

	int_event.any.type = li_event_window_close;
	int_event.any.window.p = window;
	li_cocoa_win_cb(&int_event);
	
	return NO;
}
@end

@implementation LiView
- (instancetype) _init_with_window:(LiCocoaWindow*)init_window {
	self = [super init];
	window = init_window;
	return self;
}

+ (int) _get_mouse_state {
	int state = 0;
	int pressed = [NSEvent pressedMouseButtons];

	if (pressed & 1 << 0)
		state |= li_button_state_left;
	if (pressed & 2 << 0)
		state |= li_button_state_right;
	return state;
}

- (void) _handle_button_event:(NSEvent*)event button:(int)eventButton pressed:(int) isPressed {
	li_event_t int_event;
	NSPoint point = [event locationInWindow];

	int_event.any.window.p = [event window];
	if (isPressed)
		int_event.any.type = li_event_button_press;
	else
		int_event.any.type = li_event_button_release;
	int_event.button.button = eventButton;
	int_event.button.x = point.x;
	int_event.button.y = point.y;
	int_event.button.state = li_win_xlat_key_state([event modifierFlags]) | [LiView _get_mouse_state];
	li_cocoa_win_cb(&int_event);
}

- (void) _handle_mouse_move_event:(NSEvent*)event {
	li_event_t int_event;
	NSPoint point = [event locationInWindow];

	int_event.any.window.p = [event window];
	int_event.any.type = li_event_motion_notify;
	int_event.motion.x = point.x;
	int_event.motion.y = point.y;
	int_event.motion.state = li_win_xlat_key_state([event modifierFlags]) | [LiView _get_mouse_state];
	li_cocoa_win_cb(&int_event);
}

- (void) leftMouseDown: (NSEvent*) event {
	[self _handle_button_event:event button:li_button_left pressed:1];
}

- (void) rightMouseDown: (NSEvent*) event {
	[self _handle_button_event:event button:li_button_right pressed:1];
}

- (void) rightMouseUp: (NSEvent*) event {
	[self _handle_button_event:event button:li_button_right pressed:0];
}

- (void) otherMouseDown: (NSEvent*) event {
	[self _handle_button_event:event button:li_button_unknown pressed:1];
}

- (void) otherMouseUp: (NSEvent*) event {
	[self _handle_button_event:event button:li_button_unknown pressed:0];
}

- (void) mouseDown: (NSEvent *) event {
	[self _handle_button_event:event button:li_button_left pressed:1];
}

- (void) mouseUp: (NSEvent *) event {
	[self _handle_button_event:event button:li_button_left pressed:0];
}

- (void) keyDown: (NSEvent *) event {
	li_event_t int_event;

	if ([event isARepeat])
		int_event.any.type = li_event_key_repeat;
	else
		int_event.any.type = li_event_key_press;
	int_event.any.window.p = [event window];
	int_event.key.key = li_win_xlat_key([event keyCode]);
	int_event.key.state = li_win_xlat_key_state([event modifierFlags]) | [LiView _get_mouse_state];
	li_cocoa_win_cb(&int_event);
}

- (void) keyUp: (NSEvent *) event {
	li_event_t int_event;

	int_event.any.type = li_event_key_release;
	int_event.any.window.p = [event window];
	int_event.key.key = li_win_xlat_key([event keyCode]);
	int_event.key.state = li_win_xlat_key_state([event modifierFlags]) | [LiView _get_mouse_state];
	li_cocoa_win_cb(&int_event);
}

- (void) mouseMoved: (NSEvent *) event {
	[self _handle_mouse_move_event:event];
}

- (void) mouseDragged: (NSEvent *) event {
	[self _handle_mouse_move_event:event];
}

- (void) rightMouseDragged: (NSEvent *) event {
	[self _handle_mouse_move_event:event];
}

- (void) otherMouseDragged: (NSEvent *) event {
	[self _handle_mouse_move_event:event];
}

@end

int _setup_nsgl(void) {
	if (li_cocoa_framework)
		return 1;
	li_cocoa_framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
	return li_cocoa_framework != NULL;
}

void li_win_init(win_cb_proc_t cb) {
	li_cocoa_win_cb = cb;
	[NSApplication sharedApplication];
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	li_assert(_setup_nsgl());
}

void li_win_exit(void) {

}

void li_win_poll(void) {
	NSEvent* event;

	while (1) {
		event = [NSApp nextEventMatchingMask:NSEventMaskAny
			untilDate:[NSDate distantPast]
			inMode:NSDefaultRunLoopMode
			dequeue:YES];

		if (event == nil)
			break;

		[NSApp sendEvent:event];
	}
}

li_win_t li_win_create(int width, int height) {
	li_win_t window;
	NSRect window_rect;
	LiCocoaWindow *native_window;
	LiCocoaWindowDelegate *delegate;
	LiView *view;

	window_rect = NSMakeRect(0, 0, width, height);
	native_window = [[LiCocoaWindow alloc] initWithContentRect:window_rect
		styleMask:li_cocoa_win_style
		backing:NSBackingStoreBuffered
		defer:NO];
	delegate = [[LiCocoaWindowDelegate alloc] _init_with_window:native_window];
	view = [[LiView alloc] _init_with_window:native_window];
	[native_window setContentView:view];
	[native_window makeFirstResponder:view];
	[native_window setDelegate:delegate];
	[native_window setAcceptsMouseMovedEvents:YES];
	[native_window setRestorable:NO];
	window.p = native_window;
	return window;
}

void li_win_destroy(li_win_t win) {
	LiCocoaWindow *native_window;

	native_window = win.p;
	[[native_window contentView] release];
	[[native_window delegate] release];
	[native_window close];
}

void li_win_map(li_win_t win) {
	LiCocoaWindow *native_window;

	native_window = win.p;
	[native_window makeKeyAndOrderFront:nil];
}

void li_win_set_data(li_win_t win, void *data) {
	[(LiCocoaWindow*) win.p setUser_pointer:data];
}

void *li_win_get_data(li_win_t win) {
	return [(LiCocoaWindow*) win.p user_pointer];
}

li_ctx_t li_ctx_create(li_win_t win, int version) {
	LiCocoaWindow *window;
	window = win.p;

	NSOpenGLPixelFormatAttribute gl_version = NSOpenGLProfileVersionLegacy;
	if (version >= 3)
		gl_version = NSOpenGLProfileVersion3_2Core;
	if (version >= 4)
		gl_version = NSOpenGLProfileVersion4_1Core;

	NSOpenGLPixelFormatAttribute attribs[] = {
		NSOpenGLPFAMultisample,
		NSOpenGLPFASampleBuffers, 0,
		NSOpenGLPFASamples, 0,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 32,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFAOpenGLProfile, gl_version,
		0
	};
	NSOpenGLPixelFormat *pfd = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	li_assert(pfd != 0);

	NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:pfd shareContext:nil];
	[[window contentView] setWantsBestResolutionOpenGLSurface:false];
	[context setView:[window contentView]];
	
	assert(context != nil);

	li_ctx_t ctx;
	ctx.p = context;
	return ctx;
}

void li_ctx_destroy(li_win_t win, li_ctx_t ctx) {
	(void) win;
	[(NSOpenGLContext *) ctx.p dealloc];
}

void li_ctx_make_current(li_win_t win, li_ctx_t ctx) {
	NSOpenGLContext *context;
	(void) win;
	context = ctx.p;
	[context makeCurrentContext];
}

void li_ctx_swap_buffers(li_win_t win, li_ctx_t ctx) {
	NSOpenGLContext *context;
	(void) win;
	context = ctx.p;
	[context flushBuffer];
}

void *li_ctx_get_proc_addr(const char *name) {
	CFStringRef symbol_name = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingASCII);
	void *symbol = CFBundleGetFunctionPointerForName(li_cocoa_framework, symbol_name);
	CFRelease(symbol_name);
	return symbol;
}
