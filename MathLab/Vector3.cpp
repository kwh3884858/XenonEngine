//  Vector3.cpp
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright © 2018 whkong. All rights reserved.
#include "Vector3.h"

namespace MathLab {
    Vector3::Vector3():
        x(0),
        y(0),
        z(0)
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3::Vector3(int ax, int ay, int az) {
        x = ax;
        y = ay;
        z = az;
    }

    Vector3::Vector3(const Vector3& para) {
        this->x = para.x;
        this->y = para.y;
        this->z = para.z
    }

    Vector3::~Vector3() {

    }


    void Vector3::Swap() {
        int temp = x;
        x = y;
        y = temp;
    }

    Vector3 operator+(const Vector3& v1, const Vector3& v2) {
        Vector3 vector;
        vector.x = v1.x + v2.x;
        vector.y = v1.y + v2.y;
        vector.z = v1.z + v2.z;
        return  vector;
    }

    Vector3 operator-(const Vector3& v1, const Vector3& v2) {
        Vector3 vector;
        vector.x = v1.x - v2.x;
        vector.y = v1.y - v2.y;
        vector.z = v1.z - v2.z;
        return  vector;
    }

    void SwapVector(Vector3* vectorA, Vector3* vectorB) {
        Vector3 temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;
        vectorA->z = vectorB->z;

        vectorB->x = temp.x;
        vectorB->y = temp.y;
        vectorB->z = temp.z;

    }


    bool LessY(const Vector3& origin, const Vector3& compare) {
        return origin.y < compare.y;
    }

    void Exchange(Vector3*const a, Vector3*const b) {
        Vector3 temp = *a;
        a->x = b->x;
        a->y = b->y;
        a->z = b->z;
        b->x = temp.x;
        b->y = temp.y;
        b->z = temp.z;
    }

}
