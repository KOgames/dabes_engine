//
//  DebugMenuViewController.h
//  DaBesEngine
//
//  Created by Hunter Bridges on 6/4/13.
//  Copyright (c) 2013 The Telemetry Group. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <DaBes-iOS/DaBes-iOS.h>
#import <GameKit/GameKit.h>

typedef enum {
  GraphicalResourceKindSprite = 0,
  GraphicalResourceKindTileset = 1
} GraphicalResourceKind;

@class SDKiOSProjectEngineViewController;
@interface DebugMenuViewController : UIViewController
    <GKMatchmakerViewControllerDelegate, GKMatchDelegate>

@property (nonatomic, strong, readonly) UIScrollView *scrollView;
@property (nonatomic, strong) SDKiOSProjectEngineViewController *engineVC;
@property (nonatomic, assign) Input *touchInput;

- (CGFloat)height;
- (int)pages;
- (void)log:(NSString *)fmt arguments:(va_list)arguments;

@end
