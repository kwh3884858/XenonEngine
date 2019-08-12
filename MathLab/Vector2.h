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
struct Vector2{
    int x;
    int y;


public:
    friend void SwapVector(Vector2* vectorA, Vector2* vectorB);
    friend bool LessY(const Vector2& origin, const Vector2& compare);
    friend void Exchange(Vector2*const a, Vector2*const b);

    friend Vector2 operator+(const Vector2& v1,const Vector2& v2);
    friend Vector2 operator-(const Vector2& v1,const Vector2& v2);

    Vector2();
    Vector2(int ax, int ay);
    Vector2(const Vector2&);
    ~Vector2();

    void Swap();


};

Vector2 operator+(const Vector2& v1,const Vector2& v2);
Vector2 operator-(const Vector2& v1,const Vector2& v2);

void SwapVector(Vector2* vectorA, Vector2* vectorB);
bool LessY(const Vector2& origin, const Vector2& compare);
void Exchange(Vector2*const a, Vector2*const b);
}
#endif /* VectorStruct_h */
