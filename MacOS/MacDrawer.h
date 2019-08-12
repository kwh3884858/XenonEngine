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
#include "../MathLab/Vector2.h"
#include "../MathLab/ViewStruct.h"
#import "UICanvas.h"

using MathLab::Vector2;
using MathLab::ViewStruct;

class MacDrawer {
    ViewStruct *m_viewStrcut;
    UICanvas *m_uiCanvas;

    Vector2 m_min,m_max;
private:
    bool LineClip(Vector2 * aStart, Vector2 * aEnd);
    char Encode(const Vector2& point);
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
    void DrawPixel(Vector2 point);
    void DrawLine(const Vector2& start, const Vector2& end);
    void DrawTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3);
};

#endif /* MacDrawer_h */
