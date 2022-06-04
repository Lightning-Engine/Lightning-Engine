//
//  AppDelegate.h
//  LightTest
//
//  Created by Daan Meijer on 29/05/2022.
//

#import <UIKit/UIKit.h>
#import "li/view/impl.h"

@interface IOSAppDelegate : UIResponder <UIApplicationDelegate>

@property(strong, nonatomic) UIWindow *window;

@end

@interface IOSView : UIView

@end

@interface IOSViewController : UIViewController

@end

int li_view_ios_init(void);
void li_view_ios_poll(void);
