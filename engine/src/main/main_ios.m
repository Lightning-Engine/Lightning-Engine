#import "li/view/ios.h"

#import <UIKit/UIKit.h>

int main(int argc, char **argv) {
    NSString *appDelegateClassName;
    @autoreleasepool {
        appDelegateClassName = NSStringFromClass([IOSAppDelegate class])
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
