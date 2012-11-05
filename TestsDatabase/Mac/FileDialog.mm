#import <Cocoa/Cocoa.h>

#include <iostream>

std::string OpenFileDialog()
{
    NSOpenPanel *panel	= [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:NO];
    [panel setAllowedFileTypes:[NSArray arrayWithObject:@"qdb"]];
    if([panel runModal] != NSOKButton){
        return "";
    }
    
    return [[[panel URL] path] UTF8String];
}

std::string SaveFileDialog()
{
    NSSavePanel *panel	= [NSSavePanel savePanel];
    [panel setAllowedFileTypes:[NSArray arrayWithObject:@"qdb"]];
    if([panel runModal] != NSOKButton){
        return "";
    }
    
    return [[[panel URL] path] UTF8String];
}
