//  Matrix2.h
//  XenonEngine
//
//  Created by whkong on 2021-5-5.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "Vector2.h"
namespace MathLab {

    template<typename T>
    struct Matrix2;

    typedef Matrix2<Vector2f> Matrix2X2;

    template<typename T>
    Vector2<T> operator*(const Vector2<T>& vec, const Matrix2<Vector2<T>>& matrix);

    template<typename T>
    struct Matrix2 final {
        T v1;
        T v2;

    public:
        Matrix2();
        Matrix2(const T& aV1, const T& aV2);
        Matrix2(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9);
        Matrix2(const Matrix2& aMatrix);
        ~Matrix2();
    };

    template<typename T>
    Vector2<T>
        operator*(const Vector2<T>& vec, const Matrix2<Vector2<T>>& matrix)
    {
        Vector2<T> result;
        result.x = vec.x * matrix.v1.x + vec.y * matrix.v2.x;
        result.y = vec.x * matrix.v1.y + vec.y * matrix.v2.y;
        return result;
    }

    template<typename T>
    T operator*(const T& lValue, const Matrix2<T>& rValue);

    template<typename T>
    Matrix2<T>::Matrix2()
    {
    }

    template<typename T>
    Matrix2<T>::Matrix2(const T& aV1, const T& aV2)
    {
        v1 = aV1;
        v2 = aV2;
    }

    template<typename T>
    Matrix2<T>::Matrix2(const Matrix2<T>& aMatrix)
    {
        v1 = aMatrix.v1;
        v2 = aMatrix.v2;
    }

    template<typename T>
    Matrix2<T>::~Matrix2()
    {

    }

    template<typename T>
    T operator*(const T& lValue, const Matrix2<T>& rValue)
    {
        T result;
        result.x = lValue.x * rValue.v1.x + lValue.y * rValue.v2.x;
        result.y = lValue.x * rValue.v1.y + lValue.y * rValue.v2.y;
        return result;
    }

}