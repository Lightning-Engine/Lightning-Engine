#import "li/view/ios.h"

@interface IOSAppDelegate ()

@end

@implementation IOSAppDelegate

- (BOOL)application:(UIApplication *) application
    didFinishLaunchingWithOptions:(NSDictionary *) launchOptions {
    li_main();
    return YES;
}

@end
