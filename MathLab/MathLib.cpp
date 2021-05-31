//
//  MathLib.cpp
//  MacWindow
//
//  Created by 威化饼干 on 14/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//
#include <cmath>

#include "MathLabDefinition.h"

#include "MathLib.h"
#include "Matrix3.h"
#include "Matrix2.h"

namespace MathLab {

    float DegreeToRadians(float degree)
    {
        return degree * TO_RADIAN;
    }

    Vector3f RotateXAxis(const Vector3f& vec, const float degree)
    {
        float radian = DegreeToRadians(degree );
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
        float radius = DegreeToRadians(degree);
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
        float radius = DegreeToRadians(degree);
        Matrix3 rotationMatrix(
            Vector3f(cos(radius), sin(radius), 0),
            Vector3f(-sin(radius), cos(radius), 0),
            Vector3f(0, 0, 1)
        );
        Vector3f result = vec * rotationMatrix;
        return result;
    }

    Vector2f Rotate2D(const Vector2f& vec, const float degree)
    {
        float radius = DegreeToRadians(degree);
        Matrix2X2 rotationMatrix(
            Vector2f(cos(radius), sin(radius)),
            Vector2f(-sin(radius), cos(radius))
        );
        Vector2f result = vec * rotationMatrix;
        return result;
    }

}