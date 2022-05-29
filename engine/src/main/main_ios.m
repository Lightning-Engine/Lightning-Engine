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
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
