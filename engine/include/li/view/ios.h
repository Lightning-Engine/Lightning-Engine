//
//  AppDelegate.h
//  LightTest
//
//  Created by Daan Meijer on 29/05/2022.
//

#import "li/view/impl.h"

#import <UIKit/UIKit.h>

@interface IOSAppDelegate : UIResponder <UIApplicationDelegate>

@property(strong, nonatomic) UIWindow *window;

@end

@interface IOSView : UIView

@end

int  li_view_ios_init(void);
void li_view_ios_poll(void);
