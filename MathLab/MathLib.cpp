//
//  MathLib.cpp
//  MacWindow
//
//  Created by 威化饼干 on 14/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//
#include <cmath>

#include "Matrix3.h"
#include "MathLib.h"
#include "MathLabDefinition.h"

namespace MathLab {
    Vector3f RotateXAxis(const Vector3f& vec, const float degree)
    {
        float radian = degree * TO_RADIAN;
        Matrix3 rotationMatrix(
            Vector3f(1, 0, 0),
            Vector3f(0, cos(radian), sin(radian)),
            Vector3f(0, -sin(radian), cos(radian))
        );
        Vector3f result = vec * rotationMatrix;
        return result;
    }

    Vector3f RotateYAxis(const Vector3f& vec, const float degree)
    {
        float radius = degree * TO_RADIAN;
        Matrix3 rotationMatrix(
            Vector3f(cos(radius), 0, sin(radius)),
            Vector3f(0, 1, 0),
            Vector3f(-sin(radius), 0, cos(radius))
        );
        Vector3f result = vec * rotationMatrix;
        return result;
    }

    Vector3f RotateZAxis(const Vector3f& vec, const float degree)
    {
        float radius = degree * TO_RADIAN;
        Matrix3 rotationMatrix(
            Vector3f(cos(radius), sin(radius), 0),
            Vector3f(-sin(radius), cos(radius), 0),
            Vector3f(0, 0, 1)
        );
        Vector3f result = vec * rotationMatrix;
        return result;
    }
}