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

    Vector3::Vector3(float ax, float ay, float az) {
        x = ax;
        y = ay;
        z = az;
    }

    Vector3::Vector3(const Vector3& para) {
        this->x = para.x;
        this->y = para.y;
        this->z = para.z;
    }

    Vector3::~Vector3() {

    }


    void Vector3::Swap() {
        float temp = x;
        x = y;
        y = temp;
    }

    Vector3& Vector3::operator+=(const Vector3& rvalue)
    {
        this->x += rvalue.x;
        this->y += rvalue.y;
        this->z += rvalue.z;
        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& rvalue)
    {
        this->x -= rvalue.x;
        this->y -= rvalue.y;
        this->z -= rvalue.z;
        return *this;
    }

    float Vector3::dot(const Vector3& vec) const
    {
        return this->x * vec.x + this->y * vec.y + this->z * vec.z;
    }

    Vector3 operator+(const Vector3& v1, const Vector3& v2) {
        Vector3 vector(v1);
        vector += v2;
        return  vector;
    }

    Vector3 operator-(const Vector3& v1, const Vector3& v2) {
        Vector3 vector(v1);
        vector -= v2;
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


