//
//  MacDrawer.h
//  MacWindow
//
//  Created by 威化饼干 on 7/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#ifndef MacDrawer_h
#define MacDrawer_h

#include <stdio.h>
#include "../MathLab/Vector2f.h"
#include "../MathLab/ViewStruct.h"
#import "UICanvas.h"

using MathLab::Vector2i;
using MathLab::ViewStruct;

class MacDrawer {
    ViewStruct *m_viewStrcut;
    UICanvas *m_uiCanvas;

    Vector2i m_min,m_max;
private:
    bool LineClip(Vector2i * aStart, Vector2i * aEnd);
    char Encode(const Vector2i& point);
    char And(const char & a, const char & b);
    void SwapCode(char * codeA, char * codeB);
//    void SwapVector(Vector2* vectorA, Vector2* vectorB);

public:
    MacDrawer();
    MacDrawer(UICanvas * aCanvasController);
    MacDrawer(UICanvas * aCanvasController,
              ViewStruct * aViewStruct);
    void SetCanvasController(UICanvas* aCanvasController);
    void TestDraw();
    ~MacDrawer();
    void SetViewStrcut(const ViewStruct& viewStruct);
    void SetColor(const char& R, const char& G, const char& B,const char& A);
    void DrawPixel(int x, int y);
    void DrawPixel(Vector2i point);
    void DrawLine(const Vector2i& start, const Vector2i& end);
    void DrawTriangle(const Vector2i& p1, const Vector2i& p2, const Vector2i& p3);
};

#endif /* MacDrawer_h */
