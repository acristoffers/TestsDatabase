/* (c) 2012 Álan Crístoffer */

#import "AppDelegate.h"

#import "include/cef_app.h"
#import "include/cef_application_mac.h"
#import "include/cef_base.h"
#import "include/cef_browser.h"
#import "include/cef_client.h"
#import "include/cef_command_line.h"
#import "include/cef_frame.h"
#import "include/cef_runnable.h"
#import "include/cef_web_plugin.h"
#import "include/cef_web_urlrequest.h"

#import "ClientHandler.h"
#import "AppHandler.h"

#include <iostream>

extern std::string OpenFileDialog();
extern std::string SaveFileDialog();

@implementation AppDelegate

@synthesize window;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [[NSApplication sharedApplication] setPresentationOptions:NSFullScreenWindowMask];
  
    [window setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary];
  
    // comes first because ClientHandler needs it when created;
    std::string path = [[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSUTF8StringEncoding];
    AppHandler::htmlFolderPath = path + "/html";
    path = "file://" + path + "/html/index.html";

    CefSettings appSettings;
    CefBrowserSettings browserSettings;
    
    CefRefPtr<CefApp> cefApplication;
    CefRefPtr<CefClient> client(new ClientHandler());
    
    CefWindowInfo info;
    info.SetAsChild([window contentView], 0, 0, [[window contentView] frame].size.width, [[window contentView] frame].size.height);
    
    CefInitialize(appSettings, cefApplication);
    
    CefBrowser::CreateBrowser(info, client, path, browserSettings);
    
    CefRunMessageLoop();
}

- (BOOL)application:(NSApplication *)sender openFile:(NSString *)filename
{
    AppHandler::instance()->openDataBase([filename cStringUsingEncoding:NSUTF8StringEncoding]);
    return true;
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (IBAction)openAction:(id)sender
{
    AppHandler::instance()->openDataBase(OpenFileDialog());
}

- (IBAction)saveAction:(id)sender
{
    AppHandler::instance()->openDataBase(SaveFileDialog());
}

@end
