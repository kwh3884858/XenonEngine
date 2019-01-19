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
struct Vector2{
    int x;
    int y;

private:

public:
    static void SwapVector(Vector2* vectorA, Vector2* vectorB);

    static bool LessY(const Vector2& origin, const Vector2& compare);
    static void Exchange(Vector2*const a, Vector2*const b);
    Vector2();
    Vector2(int ax, int ay);
    Vector2(const Vector2&);
    void Swap();
    Vector2 operator+(const Vector2& v);
    ~Vector2();
};
//
//struct Vector2f{
//    float x;
//    float y;
//
//public:
//    Vector2f();
//    Vector2f(float ax,float ay);
//
//    Vector2f operator+(const Vector2f& v);
//    ~Vector2f();
//};
//
//
//struct Vector3{
//    int x;
//    int y;
//    int z;
//};

#endif /* VectorStruct_h */
