#import "Cocoa/Cocoa.h"
#import "QuartzCore/CVDisplayLink.h"
#import "OpenGL/OpenGL.h"
#import <mach-o/dyld.h>
#import <stdlib.h>
#import <string.h>
#include "li/win.h"
#include "li/keymap.h"
#include "li/assert.h"
#include <OpenGL/gl.h>
#include <stdio.h>

static const NSUInteger LI_WINDOW_STYLE = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
static CVReturn dlCallBack(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*, CVOptionFlags, CVOptionFlags*, void*);
static win_cb_proc_t li_win_cb;

@interface LiWindow : NSWindow {
}
@end

@interface LiWindowDelegate : NSObject <NSWindowDelegate> {
	LiWindow *window;
}
- (instancetype)initWithWindow:(LiWindow*)initWindow;
@end

@interface LiView : NSOpenGLView {
@public
	LiWindow *window;
	CVDisplayLinkRef _displayLink;
}
@property CVDisplayLinkRef displayLink;

- (instancetype)initWithWindow:(LiWindow*)initWindow;
- (void) destroyLiView;
- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime;
- (void)handleButtonEvent:(NSEvent*)event button:(int)eventButton pressed:(int) isPressed;
@end

@implementation LiWindow
- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)canBecomeKeyView {
	return YES;
}
@end

@implementation LiWindowDelegate
- (instancetype)initWithWindow:(LiWindow*)initWindow {
	self = [super init];
	window = initWindow;
	return self;
}

- (void) windowDidResize:(NSNotification *) notification {
	li_event_t int_event;
	NSSize size = [[window contentView] frame].size;

	int_event.any.type = li_event_window_resize;
	int_event.any.window.p = window;
	int_event.resize.width = (int) size.width;
	int_event.resize.height = (int) size.height;
	li_win_cb(&int_event);
}

- (BOOL) windowShouldClose:(NSNotification *)notification {
	li_event_t int_event;

	int_event.any.type = li_event_close;
	int_event.any.window.p = window;
	li_win_cb(&int_event);

	
	return NO;
}
@end

@implementation LiView
@synthesize displayLink;

- (instancetype)initWithWindow:(LiWindow*)initWindow {
	NSOpenGLPixelFormatAttribute attribs[] = {
		NSOpenGLPFAMultisample,
		NSOpenGLPFASampleBuffers, 0,
		NSOpenGLPFASamples, 0,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAColorSize, 32,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
		0
	};
	NSOpenGLPixelFormat *pfd = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	li_assert(pfd != 0);

	self = [super initWithFrame:[[initWindow contentView] frame] pixelFormat:[pfd autorelease]];
	window = initWindow;
	return self;
}

- (void)handleButtonEvent:(NSEvent*)event button:(int)eventButton pressed:(int) isPressed {
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
	int_event.button.state = 0;
	li_win_cb(&int_event);
}

- (void) leftMouseDown: (NSEvent*) event {
	[self handleButtonEvent:event button:li_button_left pressed:1];
}

- (void) rightMouseDown: (NSEvent*) event {
	[self handleButtonEvent:event button:li_button_right pressed:1];
}

- (void) leftMouseUp: (NSEvent*) event {
	[self handleButtonEvent:event button:li_button_left pressed:0];
}

- (void) rightMouseUp: (NSEvent*) event {
	[self handleButtonEvent:event button:li_button_right pressed:0];
}

- (void) otherMouseDown: (NSEvent*) event {
	[self handleButtonEvent:event button:li_button_unknown pressed:1];
}

- (void) otherMouseUp: (NSEvent*) event {
	[self handleButtonEvent:event button:li_button_unknown pressed:0];
}

- (void) mouseDown: (NSEvent *) event {
	[self handleButtonEvent:event button:li_button_left pressed:1];
}

- (void) mouseUp: (NSEvent *) event {
	[self handleButtonEvent:event button:li_button_left pressed:0];
}

- (void) keyDown: (NSEvent *) event {
	li_event_t int_event;

	if ([event isARepeat])
		int_event.any.type = li_event_key_repeat;
	else
		int_event.any.type = li_event_key_press;
	int_event.any.window.p = [event window];
	int_event.key.key = li_win_xlat_key([event keyCode]);
	int_event.key.state = li_win_xlat_key_state([event modifierFlags]);
	li_win_cb(&int_event);
}

- (void) keyUp: (NSEvent *) event {
	li_event_t int_event;

	int_event.any.type = li_event_key_release;
	int_event.any.window.p = [event window];
	int_event.key.key = li_win_xlat_key([event keyCode]);
	int_event.key.state = li_win_xlat_key_state([event modifierFlags]);
	li_win_cb(&int_event);
}

- (void) mouseMoved: (NSEvent *) event {
	li_event_t int_event;
	NSPoint point = [event locationInWindow];

	int_event.any.type = li_event_motion_notify;
	int_event.any.window.p = [event window];
	int_event.motion.x = point.x;
	int_event.motion.y = point.y;
	int_event.motion.state = 0;
	li_win_cb(&int_event);
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime {
	return kCVReturnSuccess;
}

- (void) prepareOpenGL {
	[super prepareOpenGL];

	[[self openGLContext] makeCurrentContext];
	int vsync = 1;
	[[self openGLContext] setValues:&vsync forParameter:NSOpenGLContextParameterSwapInterval];
	printf("reached prepareOpenGL\n");

	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	CVDisplayLinkSetOutputCallback(displayLink, &dlCallBack, self);
	CVDisplayLinkStart(displayLink);

	CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];
	CGLPixelFormatObj pfdObj = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, pfdObj);

	int dim[2] = {[self frame].size.width, [self frame].size.height};
	CGLSetParameter(cglContext, kCGLCPSurfaceBackingSize, dim);
	CGLEnable(cglContext, kCGLCERasterization);

	CGLLockContext(cglContext);

	printf("%s\n", glGetString(GL_VERSION));

	CGLUnlockContext(cglContext);

	CVDisplayLinkStart(displayLink);
}

- (void) destroyLiView {
	CVDisplayLinkStop(displayLink);
	CVDisplayLinkRelease(displayLink);
}
@end

void li_win_init(win_cb_proc_t cb) {
	li_win_cb = cb;
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
	NSRect windowRect;
	LiWindow *native_window;
	LiWindowDelegate *delegate;
	LiView *view;

	windowRect = NSMakeRect(0, 0, width, height);
	native_window = [[LiWindow alloc] initWithContentRect:windowRect
		styleMask:LI_WINDOW_STYLE
		backing:NSBackingStoreBuffered
		defer:NO];
	view = [[LiView alloc] initWithWindow:native_window];
	delegate = [[LiWindowDelegate alloc] initWithWindow:native_window];
	[native_window setContentView:view];
	[native_window makeFirstResponder:view];
	[native_window setDelegate:delegate];
	[native_window setAcceptsMouseMovedEvents:YES];
	[native_window setRestorable:NO];
	window.p = native_window;
	return window;
}

void li_win_destroy(li_win_t win) {
	LiWindow *native_window;

	native_window = win.p;
	[(LiView*) [native_window contentView] destroyLiView];
	[[native_window contentView] release];
	[[native_window delegate] release];
	[native_window close];
}

void li_win_map(li_win_t win) {
	LiWindow *native_window;

	native_window = win.p;
	[native_window makeKeyAndOrderFront:nil];
}

li_ctx_t li_ctx_create(li_win_t win) {
	li_ctx_t context;

	context.p = 0;
	(void)win;
	return context;
}

void li_ctx_destroy(li_ctx_t ctx) {
	(void)ctx;
}

void li_ctx_make_current(li_win_t win, li_ctx_t ctx) {
	(void)win;
	(void)ctx;
}

void li_ctx_swap_buffers(li_win_t win) {
	(void)win;
}

void *li_ctx_get_proc_addr(const char *name) {
	char *symbol_name = malloc(strlen(name) + 2);
	strcpy(symbol_name + 1, name);
	symbol_name[0] = '_';
	NSSymbol symbol = NULL;
	if (NSIsSymbolNameDefined(symbol_name))
		symbol = NSLookupAndBindSymbol(symbol_name);
	free(symbol_name);
	return symbol ? NSAddressOfSymbol(symbol) : NULL;
}

static CVReturn dlCallBack(CVDisplayLinkRef displayLink, const CVTimeStamp* now,
	const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* context) {
	(void)now;
	(void)flagsIn;
	(void)flagsOut;
	(void)displayLink;
	CVReturn result = [(LiView*)context getFrameForTime:outputTime];
	return result;
}
