//  Vector2.h
//  XenonEngine
//
//  Created by whkong on 2021-4-14.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#ifndef VectorStruct_h
#define VectorStruct_h

#include <math.h> // For sqrt
//#include <stdio.h>
namespace MathLab {

    template<typename T>
    struct Vector2;

    typedef Vector2<float> Vector2f;

    template<typename T>
    Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2);
    template<typename T>
    Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2);

    template<typename T>
    void SwapVector(Vector2<T>& vectorA, Vector2<T>& vectorB);
    template<typename T>
    bool LessY(const Vector2<T>& origin, const Vector2<T>& compare);

    template<typename T>
    struct Vector2 final {
        T x;
        T y;

    public:
        static Vector2 Zero;

        friend void SwapVector<T>(Vector2<T>& vectorA, Vector2<T>& vectorB);
        friend bool LessY<T>(const Vector2<T>& origin, const Vector2<T>& compare);

        friend Vector2<T> operator+ <>(const Vector2<T>& v1, const Vector2<T>& v2);
        friend Vector2<T> operator- <>(const Vector2<T>& v1, const Vector2<T>& v2);

        Vector2 operator-()const;
        Vector2& operator=(const Vector2& value);
        Vector2& operator+=(const Vector2& value);
        Vector2& operator-=(const Vector2& value);

        Vector2 operator*(T value)const;
        Vector2 operator/(T value)const;

        T Dot(const Vector2& vec)const;
        T Cross(const Vector2& vec)const;

        Vector2();
        Vector2(T ax, T ay);
        Vector2(const Vector2&);
        ~Vector2();

        void Swap();
        Vector2 Normalize()const;
        T Magnitude()const;
        T DoubleMagnitude()const;
    };

    template<typename T>
    __declspec(selectany) Vector2<T> Vector2<T>::Zero = Vector2f(0, 0);


    template<typename T>
    Vector2<T>::Vector2() {
        x = 0;
        y = 0;
    }

    template<typename T>
    Vector2<T>::Vector2(T ax, T ay) {
        x = ax;
        y = ay;
    }

    template<typename T>
    Vector2<T>::Vector2(const Vector2& para) {
        this->x = para.x;
        this->y = para.y;
    }

    template<typename T>
    Vector2<T>::~Vector2() {

    }

    template<typename T>
    Vector2<T> MathLab::Vector2<T>::operator-() const
    {
        Vector2 result(-this->x, -this->y)
    }

    template<typename T>
    Vector2<T>& Vector2<T>::operator=(const Vector2& value)
    {
        if (this == &value)
        {
            return *this;
        }
        this->x = value.x;
        this->y = value.y;
        return *this;
    }

    template<typename T>
    Vector2<T>& Vector2<T>::operator+=(const Vector2& value)
    {
        this->x += value.x;
        this->y += value.y;
        return *this;
    }

    template<typename T>
    Vector2<T>& Vector2<T>::operator-=(const Vector2& value)
    {
        this->x -= value.x;
        this->y -= value.y;
        return *this;
    }

    template<typename T>
    Vector2<T> MathLab::Vector2<T>::operator*(T value)const
    {
        Vector2<T> vector(*this);
        vector *= value;
        vector *= value;
        return vector;
    }

    template<typename T>
    Vector2<T> MathLab::Vector2<T>::operator/(T value)const
    {
        Vector2<T> vector(*this);
        vector /= value;
        vector /= value;
        return vector;
    }

    template<typename T>
    T MathLab::Vector2<T>::Dot(const Vector2& vec) const
    {
        return this->x * vec.x + this->y * vec.y;
    }

    template<typename T>
    T MathLab::Vector2<T>::Cross(const Vector2& vec) const
    {
        return this->x * vec.y - vec.x * this->y;
    }

    template<typename T>
    void Vector2<T>::Swap() {
        T temp = x;
        x = y;
        y = temp;
    }

    template<typename T>
    Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2) {
        Vector2<T> vector(v1);
        vector += v2;
        return  vector;
    }

    template<typename T>
    Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2) {
        Vector2<T> vector(v1);
        vector -= v2;
        return  vector;
    }

    template<typename T>
    void SwapVector(Vector2<T>& vectorA, Vector2<T>& vectorB)
    {
        Vector2<T> temp (vectorA);
        vectorA= vectorB;
        vectorB= temp;
    }

    template<typename T>
    bool LessY(const Vector2<T>& origin, const Vector2<T>& compare) {
        return origin.y < compare.y;
    }

    template<typename T>
    Vector2<T> MathLab::Vector2<T>::Normalize() const
    {
        T magnitide = Magnitude();
        Vector2<T> temp;
        temp->x = this->x / magnitide;
        temp->y = this->y / magnitide;
        return temp;
    }

    template<typename T>
    T MathLab::Vector2<T>::Magnitude() const
    {
        return sqrt(DoubleMagnitude());
    }

    template<typename T>
    T MathLab::Vector2<T>::DoubleMagnitude() const
    {
        return this->Dot(*this);
    }
}
#endif /* VectorStruct_h */
