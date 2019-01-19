//
//  MacDrawer.cpp
//  MacWindow
//
//  Created by 威化饼干 on 7/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#include "MacDrawer.h"

const char LEFT = 1;
const char RIGHT = 2;
const char DOWN = 4;
const char UP = 8;

MacDrawer::MacDrawer(){
    fprintf(stdout, "Controller is still null!");
    m_uiCanvas = NULL;
    m_viewStrcut = NULL;
}

MacDrawer::MacDrawer(UICanvas* uiCanvas){
    SetCanvasController(uiCanvas);
    m_viewStrcut = NULL;
}

MacDrawer::MacDrawer(UICanvas * uiCanvas,
          ViewStruct * aViewStruct){
    SetCanvasController(uiCanvas);
    SetViewStrcut(*aViewStruct);
}

void MacDrawer::SetCanvasController(UICanvas *uiCanvas){
    m_uiCanvas = uiCanvas;
}

void MacDrawer::SetViewStrcut(const ViewStruct& viewStruct){
    m_viewStrcut = new ViewStruct(*viewStruct.m_viewSize,
                                  *viewStruct.m_origin);

    m_min.x =-m_viewStrcut->m_origin->x;
    m_min.y =-m_viewStrcut->m_origin->y;
    m_max.x =m_viewStrcut->m_viewSize->x - 1 - m_viewStrcut->m_origin->x;
    m_max.y =m_viewStrcut->m_viewSize->y - 1 - m_viewStrcut->m_origin->y;
}

void MacDrawer::TestDraw(){
//    for (int i = 0 ; i < 70; i++) {
//
//        DrawPixel(Vector2(i, 20));
//    }
//    SetCenterPoint(200, 150);

    DrawLine(Vector2(0,0), Vector2(10,100));
    DrawLine(Vector2(0,0), Vector2(-50,100));
    DrawLine(Vector2(0,30), Vector2(10,-100));
    DrawLine(Vector2(0,-50), Vector2(-60,-100));
    SetColor(0, 0, 0, (char)255);
    DrawLine(Vector2(m_min.x,0), Vector2( m_max.x, 0));
    DrawLine(Vector2(0, m_min.y), Vector2(0, m_max.y));
    
}

MacDrawer::~MacDrawer(){
     [m_uiCanvas release];
     if (m_viewStrcut != NULL) {
         delete m_viewStrcut;
     }
 }

bool MacDrawer::LineClip(Vector2 *aStart, Vector2 *aEnd){
    char startCode = 0x0;
    char endCode = 0x0;
    float k ;
    startCode = Encode(*aStart);
    endCode = Encode(*aEnd);
    while(1){

        if (startCode == 0 && endCode == 0) {
            return true;
        }
        if(And(startCode, endCode) != 0){
            return false;
        }
        if(startCode == 0){
            SwapCode(&startCode, &endCode);
            Vector2::SwapVector(aStart, aEnd);
        }
        k = (float)(aEnd->y - aStart->y)/(aEnd->x - aStart->x);
        if(0 != (startCode & LEFT)){
            aStart->y = k * (m_min.x - aEnd->x) + aEnd->y;
            aStart->x = m_min.x;
        }else if (0 != (startCode & RIGHT)){
            aStart->y = k * (m_max.x - aEnd->x) + aEnd->y;
            aStart->x = m_max.x;
        }else if (0 != (startCode & DOWN)){
            if (aStart->x != aEnd->x) {
                aStart->x = ( m_min.y - aEnd->y) / k + aEnd->x;
            }
            aStart->y = m_min.y;
        }else if (0 != (startCode & UP)){
            if (aStart->x != aEnd->x) {
                aStart->x = ( m_max.y - aEnd->y) / k + aEnd->x;
            }
            aStart->y = m_max.y;
        }
        startCode = Encode(*aStart);

    }
}
char MacDrawer::Encode(const Vector2& point){
    char code = 0;
    if (point.x < m_min.x) {
        code = code | LEFT;
    }
    if (point.x > m_max.x) {
        code = code | RIGHT;
    }
    if(point.y < m_min.y){
        code = code | DOWN;
    }
    if(point.y > m_max.y){
        code = code | UP;
    }
    return code;
}

char MacDrawer::And(const char & a, const char & b){
    return a & b;
}


inline void MacDrawer::SwapCode(char * codeA, char * codeB){
    char temp = *codeA;
    *codeA = *codeB;
    *codeB = temp;
}

void MacDrawer::DrawLine(const Vector2& aStart,const Vector2& aEnd){

    Vector2 start = aStart;
    Vector2 end = aEnd;

    if (!LineClip(&start, &end)) {
        return;
    }

    int incrementX = 1;
    int incrementY = 1;

    bool steep = abs(end.y - start.y) > abs(end.x - start.x) ? true : false;
    if ( steep ) {
        start.Swap();
        end.Swap();
    }
    int deltaX = end.x - start.x;
    int deltaY = end.y - start.y;
//    float slope = abs((float)deltaY / deltaX);

    int i = start.x;
    int j = start.y;

    if(deltaY < 0 ){
        deltaY = -deltaY;
        incrementY = -1;
    }
    if(deltaX < 0 ){
        deltaX = -deltaX;
        incrementX = -1;
    }

    int error = 0;

    while (i != end.x) {
        if (!steep) {
            DrawPixel(i, j);
        }else{
            DrawPixel(j, i);
        }
        error += deltaY;
        if ( 2 * error > deltaX) {
            error -= deltaX;
            j += incrementY;

        }
        i += incrementX;
    }
}

//Edge equations
void DrawTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3){
    Vector2* points = new Vector2[3];
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;

    int length = sizeof(points)/sizeof(points[0]);
    for (int i = 0 ; i < length; i++) {
        for (int j = i; j > 0 && Vector2::LessY(points[j], points[j-1]); j--) {
            Vector2::Exchange(&points[j], &points[j -1]);
        }
    }

    if (points[0].y == points[1].y) {

    }

    if (points[1].y == points[2].y) {
        
    }

}

void FillTopFlatTriangle(const Vector2& top1, const Vector2& top2, const Vector2& buttom){

}

void FillBottomFlatTriangle(){

}

void MacDrawer::SetColor(const char& R = (char)255, const char& G = (char)255, const char& B = (char)255,const char& A = (char)255){
    [m_uiCanvas SetColorR:R G:G B:B A:A];
}

void MacDrawer::DrawPixel(int x, int y){
    if (x < m_min.x ||
        x > m_max.x ||
        y < m_min.y ||
        y > m_max.y) {
        fprintf(stderr, "Coordinate is out of range");
        return;
    }
    //    high = pixelHigh - 1 - centerY - high;
    //    wide = pixelWide - 1 - centerX + wide;
    y = m_viewStrcut->m_viewSize->y - 1 - m_viewStrcut->m_origin->y - y;
    x = m_viewStrcut->m_origin->x + x;

    [m_uiCanvas SetPixelWide:x High:y];
}

void MacDrawer::DrawPixel(Vector2 point){
    DrawPixel(point.x, point.y);
}

