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

    float RadiansToDegree(float radius)
    {
        return radius * TO_DEGREE;
    }

    Vector3f RotateXAxis(const Vector3f& vec, const float degree)
    {
        float radian = DegreeToRadians(degree );
        Matrix3f rotationMatrix(
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
        Matrix3f rotationMatrix(
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
        Matrix3f rotationMatrix(
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

	MathLab::TMatrix3X3f GetRotationTranformMatrix(const Vector3f& degree)
	{
		float radiusX = MathLab::DegreeToRadians(degree.x);
		float radiusY = MathLab::DegreeToRadians(degree.y);
		float radiusZ = MathLab::DegreeToRadians(degree.z);
		return TMatrix3X3f(
			std::initializer_list<float>{
				cos(radiusY) * cos(radiusZ),										      cos(radiusY) * sin(radiusZ),									           -sin(radiusY),
				sin(radiusX) * sin(radiusY) * cos(radiusZ) - cos(radiusX) * sin(radiusZ), sin(radiusX) * sin(radiusY) * sin(radiusZ) + cos(radiusX) * cos(radiusZ), sin(radiusX) * cos(radiusY),
				cos(radiusX) * sin(radiusY) * cos(radiusZ) + sin(radiusX) * sin(radiusZ), cos(radiusX) * sin(radiusY) * sin(radiusZ) - sin(radiusX) * cos(radiusZ), cos(radiusX) * cos(radiusY)
			}
        );
	}

    MathLab::TMatrix4X4f GetPositionAndRotationTranformMatrix(const Vector3f& position, const Vector3f& degree)
    {
        float radiusX = MathLab::DegreeToRadians(degree.x);
        float radiusY = MathLab::DegreeToRadians(degree.y);
        float radiusZ = MathLab::DegreeToRadians(degree.z);
        return TMatrix4X4f(
            std::initializer_list<float>{
                cos(radiusY) * cos(radiusZ)                                             , cos(radiusY) * sin(radiusZ)                                             , -sin(radiusY)              , 0,
                sin(radiusX) * sin(radiusY) * cos(radiusZ) - cos(radiusX) * sin(radiusZ), sin(radiusX) * sin(radiusY) * sin(radiusZ) + cos(radiusX) * cos(radiusZ), sin(radiusX) * cos(radiusY), 0,
                cos(radiusX) * sin(radiusY) * cos(radiusZ) + sin(radiusX) * sin(radiusZ), cos(radiusX) * sin(radiusY) * sin(radiusZ) - sin(radiusX) * cos(radiusZ), cos(radiusX) * cos(radiusY), 0,
                position.x                                                              , position.y                                                              , position.z                 , 1
            }
        );
    }

    Vector3f ConvertFormHomogeneous(const TVector4f& homogeneous)
    {
        if (Abs(homogeneous[3]) == 0)
        {
            return Vector3f(0, 0, 0);
        }
        Vector3f result(homogeneous[0]/ homogeneous[3], homogeneous[1]/ homogeneous[3], homogeneous[2]/ homogeneous[3]);
        return result;
    }

    TVector4f ConvertFromNonHomogeneous(const Vector3f& nonHomogeneous)
    {
        TVector4f result (nonHomogeneous);
        result[3] = 1;
        return result;
    }

    int Ceil(float value)
    {
        if (value < 0)
        {
            return (int)value;
        }
        else
        {
            if ((int)value < value)
            {
                return (int)value + 1;
            }
            else
            {
                return (int)value;
            }
        }
    }

    int Floor(float value)
    {
        return (int)value;
    }

}