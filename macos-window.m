#import <Cocoa/Cocoa.h>

int main(void) {
	NSRect frame;
	NSWindow *window;

	frame = NSMakeRect(0, 0, 640, 480);
	window = [[NSWindow alloc] initWithContentRect:frame
		styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask
		backing:NSBackingStoreBuffered
		defer:NO];
	[window makeKeyAndOrderFront:nil];
	while (1) {
		[window nextEventMatchingMask:NSAnyEventMask];
	}
	return 0;
}
