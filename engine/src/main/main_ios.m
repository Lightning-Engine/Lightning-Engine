#import <UIKit/UIKit.h>
#import "li/view/ios.h"

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([IOSAppDelegate class]);
        return UIApplicationMain(argc, argv, nil, appDelegateClassName);
    }
}
