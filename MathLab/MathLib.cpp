//
//  MathLib.cpp
//  MacWindow
//
//  Created by 威化饼干 on 14/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#include "MathLib.h"
#include "MathLabDefinition.h"

Vector3 Mathlib::RotateXAxis(const Vector3& vec, const float degree)
{
    float radian = degree * TO_RADIAN;
    Matrix3 rotationMatrix(
        Vector3(1,0,0),
        Vector3(0,sin(radian),cos(radian)),
        Vector3(0,-cos(radian),sin(radian))
    );
    Vector3 result =  vec*rotationMatrix;
    return result;
}

void Mathlib::RotateYAxis(const Vector3& vec, const float degree)
{

}

void Mathlib::RotateZAxis(const Vector3& vec, const float degree)
{

}
