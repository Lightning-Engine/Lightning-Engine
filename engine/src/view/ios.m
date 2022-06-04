//
//  AppDelegate.m
//  LightTest
//
//  Created by Daan Meijer on 29/05/2022.
//

#import "li/view/ios.h"
#import "li/main/ios.h"

const struct li_view_impl li_view_ios_impl = { li_view_ios_init, li_view_ios_poll };

@interface IOSAppDelegate ()
 
@end

@interface IOSView ()
 
@end


@implementation IOSAppDelegate

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    IOSViewController *controller = [[IOSViewController alloc] init];
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor whiteColor];
    self.window.rootViewController = controller;
    [self.window makeKeyAndVisible];
    return YES;
}

- (id)init {
    printf("here\n");
    self = [super init];
//    CGRect viewRect = CGRectMake(10, 10, 100, 100);
//    [[self inputView] addSubview:[[IOSView alloc] initWithFrame:viewRect]];
    return self;
}

- (UIResponder *) nextResponder {
    static IOSView *view;
    if (view == nil) {
        printf("Voila\n");
            CGRect viewRect = CGRectMake(10, 10, 100, 100);
            view = [[IOSView alloc] initWithFrame:viewRect];
    }
    return view;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state.
    // This can occur for certain types of temporary interruptions (such as an
    // incoming phone call or SMS message) or when the user quits the
    // application and it begins the transition to the background state. Use
    // this method to pause ongoing tasks, disable timers, and invalidate
    // graphics rendering callbacks. Games should use this method to pause the
    // game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate
    // timers, and store enough application state information to restore your
    // application to its current state in case it is terminated later.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state;
    // here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the
    // application was inactive. If the application was previously in the
    // background, optionally refresh the user interface.
}

- (void) touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    (void) event;
    printf("Thouch here\n");
}

- (void) touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    printf("Move1\n");
}

- (void) touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    printf("End\n");
}

- (void) touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    printf("Touch was cancelled\n");
}
@end

@implementation IOSView

- (instancetype)initWithFrame:(CGRect)frame {
    [super initWithFrame:frame];
    printf("init view with frame\n");
    if ([self becomeFirstResponder])
        printf("View has become the first responder\n");
    else
        printf("View is not the first responder\n");
    if ([self canBecomeFirstResponder])
        printf("I could've become first responder\n");
    else
        printf("I'll never be first responder\n");
    return self;
}

- (void) touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    (void) event;
    printf("View Thouch here\n");
}

- (void) touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    printf("View Move1\n");
}

- (void) touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    printf("View End\n");
}

- (void) touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    printf("View Touch was cancelled\n");
}
@end


@implementation IOSViewController

@end

int li_view_ios_init(void) {
    li_view_impl = &li_view_ios_impl;
    return (0);
}

void li_view_ios_poll(void) {
    return (0);
}
