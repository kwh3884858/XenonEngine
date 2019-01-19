//
//  UICanvas.m
//  MacWindow
//
//  Created by 威化饼干 on 4/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#import "UICanvas.h"

@implementation UICanvas

const int pixelWide = 400;
const int pixelHigh = 300;

int centerX = 0;
int centerY = 0;

Byte R = (char)255;
Byte G = (char)255;
Byte B = (char)255;
Byte A = (char)255;

char * bitmapData;
CGContextRef myBitmapContext ;


- (id)initWithFrame:(NSRect)frameRect{
    self = [super initWithFrame:frameRect];
    if(self){
        myBitmapContext = SPCreateBitmapContext(pixelWide, pixelHigh);
        bitmapData = CGBitmapContextGetData(myBitmapContext);

        CGContextSetRGBFillColor(myBitmapContext, 1, 0, 0, 1);
        CGContextFillRect(myBitmapContext, CGRectMake(0, 0, 200, 100));
        CGContextSetRGBFillColor(myBitmapContext, 0, 0, 1, .5 );
        CGContextFillRect(myBitmapContext, CGRectMake(0, 0, 100, 200));
    }

    return self;
}



- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    CGContextRef myContext = [[NSGraphicsContext currentContext] graphicsPort];


    CGRect myBoundingBox;
    myBoundingBox = CGRectMake(0, 0, pixelWide, pixelHigh);

    // Drawing code here.

    CGImageRef myImage = CGBitmapContextCreateImage(myBitmapContext);
    CGContextDrawImage(myContext, myBoundingBox, myImage);

    CGImageRelease(myImage);
}

- (void)SetCenterPointX:(int) acenterX Y: (int)acenterY{
    centerX = acenterX;
    centerY = acenterY;
}

- (void)SetPixelWide:(int)wide High:(int)high{
//    high = pixelHigh - centerY - high;
//    wide = pixelWide - centerX + wide;
    bitmapData[pixelWide * 4 * high + wide * 4] = R;
    bitmapData[pixelWide * 4 * high + wide * 4 + 1] = G;
    bitmapData[pixelWide * 4 * high + wide * 4 + 2] = B;
    bitmapData[pixelWide * 4 * high + wide * 4 + 3] = A;
}

- (void)removeFromSuperview{

    CGContextRelease(myBitmapContext);

    if (bitmapData) {
        free(bitmapData);
    }
}

- (void)SetColorR:(const Byte) aR G:(const Byte) aG B:( const Byte) aB A: (const Byte) aA{
    R = aR;
    G = aG;
    B = aB;
    A = aA;
}

@end

CGContextRef SPCreateBitmapContext(int pixelWide, int pixelHigh){
    CGContextRef context = NULL;
    CGColorSpaceRef colorSpace;
    void* bitmapData;
    int bitmapByteCount;
    int bitmapBytesPerRow;

    bitmapBytesPerRow  = (pixelWide * 4);
    bitmapByteCount = bitmapBytesPerRow * pixelHigh;

    colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    bitmapData = calloc(bitmapByteCount, sizeof(uint8_t));

    if(bitmapData == NULL){
        fprintf(stderr, "Memory not allocated!");
        return NULL;
    }
    context = CGBitmapContextCreate(bitmapData,
                                    pixelWide,
                                    pixelHigh,
                                    8,
                                    bitmapBytesPerRow,
                                    colorSpace,
                                    kCGImageAlphaPremultipliedLast);

    if(context == NULL){
        free(bitmapData);
        fprintf(stderr, "Cotext not created!");
        return NULL;
    }

    CGColorSpaceRelease(colorSpace);

    return context;
}
