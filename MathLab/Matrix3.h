//  Matrix3.h
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright © 2018 whkong. All rights reserved.

#pragma once

#include "Vector3.h"

namespace MathLab {
    struct Matrix3 final{
        Vector3f v1;
        Vector3f v2;
        Vector3f v3;


    public:
        Matrix3();
        Matrix3(const Vector3f& aV1, const Vector3f& aV2, const Vector3f& aV3);
        Matrix3(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9);
        Matrix3(const Matrix3& aMatrix);
        ~Matrix3();

    private:


    };

    Vector3f operator*(const Vector3f& lValue, const Matrix3& rValue);
}