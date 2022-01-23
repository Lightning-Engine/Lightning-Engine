#import <Cocoa/Cocoa.h>
#include <stdio.h>

@interface LiWindowDelegate : NSObject
{

}
@end

@implementation LiWindowDelegate
- (void) windowDidMove:(NSNotification *)notification
{
	printf("moved\n");
}
@end

@interface LiApplicationDelegate : NSObject
{

}
@end

@implementation LiApplicationDelegate
- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
	printf("Done\n");
    NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                        location:NSMakePoint(0, 0)
                                   modifierFlags:0
                                       timestamp:0
                                    windowNumber:0
                                         context:nil
                                         subtype:0
                                           data1:0
                                           data2:0];
    [NSApp postEvent:event atStart:YES];
	[NSApp stop:nil];
}
@end

int main(void) {
	NSRect frame, frame2;
	NSWindow *window, *window2;
	LiWindowDelegate *delegate, *delegate2;
	LiApplicationDelegate *app_delegate;

	app_delegate = [LiApplicationDelegate alloc];
	[NSApplication sharedApplication];
	[NSApp setDelegate:app_delegate];
	[NSApp run];

	frame = NSMakeRect(0, 0, 640, 480);
	window = [[NSWindow alloc] initWithContentRect:frame
		styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask
		backing:NSBackingStoreBuffered
		defer:NO];
	[window makeKeyAndOrderFront:nil];
	delegate = [LiWindowDelegate alloc];
	[window setDelegate:delegate];

	frame2 = NSMakeRect(0, 0, 640, 480);
	window2 = [[NSWindow alloc] initWithContentRect:frame2
		styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask
		backing:NSBackingStoreBuffered
		defer:NO];
	[window2 makeKeyAndOrderFront:nil];
	delegate2 = [LiWindowDelegate alloc];
	[window2 setDelegate:delegate2];

	while (1) {
		NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
			untilDate:[NSDate distantPast]
			inMode:NSDefaultRunLoopMode
			dequeue:YES];
		if (event != nil)
			[NSApp sendEvent:event];
	}

	return 0;
}