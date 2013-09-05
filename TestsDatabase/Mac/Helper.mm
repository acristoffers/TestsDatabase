//
// Helper.cpp
// TestsDatabase
//
// Created by Álan Crístoffer on 2/19/13.
// Copyright (c) 2013 Álan Crístoffer. All rights reserved.
//

#import "Helper.h"

#import <Cocoa/Cocoa.h>
#import <string>

void openURI(std::string uri)
{
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s", uri.c_str()]]];
}
