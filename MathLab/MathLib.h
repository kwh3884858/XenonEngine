//
//  MathLib.hpp
//  MacWindow
//
//  Created by 威化饼干 on 14/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//
#pragma once
#ifndef MathLib_hpp
#define MathLib_hpp

#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"
#include "MathLab/Matrix.h"
namespace MathLab{

    float DegreeToRadians(float degree);
     Vector3f RotateXAxis(const Vector3f& vec, const float degree);
     Vector3f RotateYAxis(const Vector3f& vec, const float degree);
     Vector3f RotateZAxis(const Vector3f& vec, const float degree);

     Vector2f Rotate2D(const Vector2f& vec, const float degree);
	 TMatrix3X3f GetRotationTranformMatrix(const Vector3f& degree);
	 TMatrix4X4f GetPositionAndRotationTranformMatrix(const Vector3f& position, const Vector3f& degree);

     Vector3f ConvertFormHomogeneous(const TVector4f& homogeneous);

     template <typename T>
     const T& clamp(const T& value, const T& min, const T& max) {
         if (value < min)
         {
             return min;
         }
         if (value > max)
         {
             return max;
         }
         return value;
     }

     template <typename T>
     T abs(const T& value)
     {
         return value < 0 ? -value : value;
     }

};
#endif /* MathLib_hpp */
