//
//  VectorStruct.h
//  MacWindow
//
//  Created by 威化饼干 on 8/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#ifndef VectorStruct_h
#define VectorStruct_h

#include <stdio.h>
namespace MathLab{
struct Vector2i final{
    int x;
    int y;


public:
    friend void SwapVector(Vector2i* vectorA, Vector2i* vectorB);
    friend bool LessY(const Vector2i& origin, const Vector2i& compare);
    friend void Exchange(Vector2i*const a, Vector2i*const b);

    friend Vector2i operator+(const Vector2i& v1,const Vector2i& v2);
    friend Vector2i operator-(const Vector2i& v1,const Vector2i& v2);

    Vector2i();
    Vector2i(int ax, int ay);
    Vector2i(const Vector2i&);
    ~Vector2i();

    void Swap();


};

Vector2i operator+(const Vector2i& v1,const Vector2i& v2);
Vector2i operator-(const Vector2i& v1,const Vector2i& v2);

void SwapVector(Vector2i* vectorA, Vector2i* vectorB);
bool LessY(const Vector2i& origin, const Vector2i& compare);
void Exchange(Vector2i*const a, Vector2i*const b);
}
#endif /* VectorStruct_h */
