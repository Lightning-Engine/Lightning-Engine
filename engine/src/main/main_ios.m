<<<<<<< HEAD
#import "li/view/ios.h"

#import <UIKit/UIKit.h>

int main(int argc, char **argv) {
    NSString *appDelegateClassName;
=======
//
//  main.m
//  LightTest
//
//  Created by Daan Meijer on 29/05/2022.
//

#import <UIKit/UIKit.h>
#import "li/view/ios.h"

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
>>>>>>> 252298aeb4de5ff113cc4ed514568e562df5a09a
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
