//
//  UICanvas.h
//  MacWindow
//
//  Created by 威化饼干 on 4/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface UICanvas : NSView

//Setting specific pixel
- (void)SetPixelWide:(int)wide High:(int)high;

//Setting color, before draw pixel
- (void)SetColorR:(const Byte) aR G:(const Byte) aG B:( const Byte) aB A: (const Byte) aA;

//Setting coordinate center
- (void)SetCenterPointX:(int) acenterX Y: (int)acenterY;

@end

CGContextRef SPCreateBitmapContext(int pixelWide, int pixelHight);

NS_ASSUME_NONNULL_END
