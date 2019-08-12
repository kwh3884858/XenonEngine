//
//  ViewController.m
//  MacWindow
//
//  Created by 威化饼干 on 3/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#import "ViewController.h"

//using MathLab::ViewStruct;

@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];

    // Do any additional setup after loading the view.

    ViewStruct* mainView = new ViewStruct(Vector2(400,300), Vector2(10,10));

    _canvs =[[UICanvas alloc]
             initWithFrame:CGRectMake(0,
                                      0,
                                      mainView->m_viewSize->x,
                                      mainView->m_viewSize->y)];

    [[self view] addSubview:_canvs];

    MacDrawer macDrawer;
    macDrawer.SetCanvasController(_canvs);
    macDrawer.SetViewStrcut(*mainView);
    macDrawer.TestDraw();

//    NSDate * now;
//    NSData* next;
//    NSData
//    *now = [NSDate date];
//
//    while(1){
//
//
//    }
//    NSDateFormatter *outputFormatter = [[NSDateFormatter alloc] init];
//    [outputFormatter setDateFormat:@"HH:mm:ss"];
//    NSString *newDateString = [outputFormatter stringFromDate:now];
//    NSLog(@"newDateString %@", newDateString);
//    [outputFormatter release];

    delete mainView;
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

- (void)viewWillDisappear{
//    [_canvs releaseGState];
}

@end

