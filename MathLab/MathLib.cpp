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
    Vector3 RotateXAxis(const Vector3& vec, const float degree)
    {
        float radian = degree * TO_RADIAN;
        Matrix3 rotationMatrix(
            Vector3(1, 0, 0),
            Vector3(0, cos(radian), sin(radian)),
            Vector3(0, -sin(radian), cos(radian))
        );
        Vector3 result = vec * rotationMatrix;
        return result;
    }

    Vector3 RotateYAxis(const Vector3& vec, const float degree)
    {
        float radius = degree * TO_RADIAN;
        Matrix3 rotationMatrix(
            Vector3(cos(radius), 0, sin(radius)),
            Vector3(0, 1, 0),
            Vector3(-sin(radius), 0, cos(radius))
        );
        Vector3 result = vec * rotationMatrix;
        return result;
    }

    Vector3 RotateZAxis(const Vector3& vec, const float degree)
    {
        float radius = degree * TO_RADIAN;
        Matrix3 rotationMatrix(
            Vector3(cos(radius), sin(radius), 0),
            Vector3(-sin(radius), cos(radius), 0),
            Vector3(0, 0, 1)
        );
        Vector3 result = vec * rotationMatrix;
        return result;
    }
}