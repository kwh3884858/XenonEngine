//  Vector3.h
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright © 2018 whkong. All rights reserved.
#pragma once


#include <stdio.h>
namespace MathLab {
    struct Vector3 final{
        float x;
        float y;
        float z;

    public:
        friend void SwapVector(Vector3* vectorA, Vector3* vectorB);
        friend bool LessY(const Vector3& origin, const Vector3& compare);
        friend void Exchange(Vector3*const a, Vector3*const b);

        friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
        friend Vector3 operator-(const Vector3& v1, const Vector3& v2);

        Vector3& operator+=(const Vector3& rvalue);
        Vector3& operator-=(const Vector3& rvalue);

        float dot(const Vector3& vec)const;

        Vector3();
        Vector3(float ax, float ay, float az);
        Vector3(const Vector3&);
        ~Vector3();

        void Swap();


    };

    Vector3 operator+(const Vector3& v1, const Vector3& v2);
    Vector3 operator-(const Vector3& v1, const Vector3& v2);

    void SwapVector(Vector3* vectorA, Vector3* vectorB);
    bool LessY(const Vector3& origin, const Vector3& compare);
    void Exchange(Vector3*const a, Vector3*const b);
}
