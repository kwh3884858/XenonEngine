//  Matrix3.h
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright © 2018 whkong. All rights reserved.

#pragma once
#include "Vector3.h"

namespace MathLab {
	template<typename T>
	struct Matrix3;

    typedef  Matrix3<float> Matrix3f;

	template<typename T>
    struct Matrix3 final{
        Vector3<T> v1;
        Vector3<T> v2;
        Vector3<T> v3;

    public:
        Matrix3();
        Matrix3(const Vector3<T>& aV1, const Vector3<T>& aV2, const Vector3<T>& aV3);
        Matrix3(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9);
        Matrix3(const Matrix3& aMatrix);
        ~Matrix3();

    private:
    };
	template<typename T>
    Vector3<T> operator*(const Vector3<T>& lValue, const Matrix3<T>& rValue);

	template<typename T>
	Matrix3<T>::Matrix3()
	{
	}

	template<typename T>
	Matrix3<T>::Matrix3(const Vector3<T>& aV1, const Vector3<T>& aV2, const Vector3<T>& aV3)
	{
		v1 = aV1;
		v2 = aV2;
		v3 = aV3;
	}

	template<typename T>
	Matrix3<T>::Matrix3(const Matrix3& aMatrix)
	{
		v1 = aMatrix.v1;
		v2 = aMatrix.v2;
		v3 = aMatrix.v3;
	}

	template<typename T>
	Matrix3<T>::~Matrix3()
	{

	}

	template<typename T>
	Vector3<T> operator*(const Vector3<T>& lValue, const Matrix3<T>& rValue)
	{
		Vector3f result;
		result.x = lValue.x * rValue.v1.x + lValue.y * rValue.v2.x + lValue.z * rValue.v3.x;
		result.y = lValue.x * rValue.v1.y + lValue.y * rValue.v2.y + lValue.z * rValue.v3.y;
		result.z = lValue.x * rValue.v1.z + lValue.y * rValue.v2.z + lValue.z * rValue.v3.z;
		return result;
	}

}