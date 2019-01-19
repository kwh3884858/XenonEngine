//
//  ViewController.m
//  MacWindow
//
//  Created by 威化饼干 on 3/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#import "ViewController.h"

#include "MacDrawer.h"
#include "Middleware/ViewStruct.h"

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

