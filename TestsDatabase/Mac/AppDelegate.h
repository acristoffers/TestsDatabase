/* (c) 2012 Álan Crístoffer */

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow* window;
}

@property (assign) IBOutlet NSWindow *window;

- (IBAction) openAction:(id)sender;
- (IBAction) saveAction:(id)sender;

@end