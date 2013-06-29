//
//  SDKScriptEditorView.h
//  DaBesEngine
//
//  Created by Hunter Bridges on 6/26/13.
//  Copyright (c) 2013 The Telemetry Group. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class SDKScriptEditorWindowController;
@class SDKScriptTabModel;
@interface SDKScriptEditorView : NSView

@property (nonatomic, copy) NSString *syntaxDefinition;
@property (nonatomic, weak) SDKScriptEditorWindowController *scriptManager;
@property (nonatomic, copy) NSString *path;
@property (nonatomic, weak) SDKScriptTabModel *tabModel;

- (id)initWithPath:(NSString *)path;

@end
