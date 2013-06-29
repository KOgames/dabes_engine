//
//  AppDelegate.h
//  DaBeSDK
//
//  Created by Hunter Bridges on 6/17/13.
//  Copyright (c) 2013 The Telemetry Group. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SDKInspectorView.h"

@class SDKScriptEditorWindowController;
@class SDKScriptManagerView;
@interface SDKAppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSWindow *inspectorWindow;
@property (assign) IBOutlet NSMenuItem *inspectorItem;
@property (assign) IBOutlet NSMenuItem *scriptEditorItem;
@property (assign) IBOutlet SDKInspectorView *inspectorView;
@property (nonatomic, strong) SDKScriptEditorWindowController *scriptEditorController;

@end
