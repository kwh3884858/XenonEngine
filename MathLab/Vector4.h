#pragma once
//  Vector4.h
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include <cassert>
#include "CrossPlatform/SColorRGBA.h"

namespace MathLab {
    template<typename T>
    struct Vector4;

    typedef     Vector4<int>        Vector4i;
    typedef     Vector4<float>      Vector4f;

    template<typename T>
    struct Vector4 final {
        T x;
        T y;
        T z;
        T w;

    public:
        friend void SwapVector(Vector4* vectorA, Vector4* vectorB);
        friend bool LessY(const Vector4& origin, const Vector4& compare);
        //friend void Exchange(Vector4*const a, Vector4*const b);

        Vector4& operator+=(const Vector4& rvalue);
        Vector4& operator-=(const Vector4& rvalue);
        Vector4& operator*=(T rvalue);
        Vector4& operator*(T rvalue) const;
        Vector4& operator/=(T rvalue);
        Vector4& operator/(T rvalue) const;

        T Dot(const Vector4& vec)const;
        Vector4 Cross(const Vector4& vec)const;

        Vector4();
        Vector4(T ax, T ay, T az, T aw);
        Vector4(const Vector4& vec);
        Vector4(const CrossPlatform::SColorRGBA& rgba);
        ~Vector4();

        bool IsNormalized()const;
        Vector4 Normalize()const;
        void Normalized();
        T Magnitude()const;
        T DoubleMagnitude()const;

        CrossPlatform::SColorRGBA ToColor()const;
    };

    template<typename T>
    Vector4<T> operator+(const Vector4<T>& v1, const Vector4<T>& v2);
    template<typename T>
    Vector4<T> operator-(const Vector4<T>& v1, const Vector4<T>& v2);

    template<typename T>
    void SwapVector(Vector4<T>* vectorA, Vector4<T>* vectorB);
    template<typename T>
    bool LessY(const Vector4<T>& origin, const Vector4<T>& compare);
    template<typename T>
    Vector4<T> operator*(const T& v1, const Vector4<T>& value);
    //template<typename T>
    //void Exchange(Vector4*const a, Vector4*const b);

    template<typename T>
    Vector4<T>::Vector4() :
        x(0),
        y(0),
        z(0),
        w(0)
    {
    }

    template<typename T>
    Vector4<T>::Vector4(T ax, T ay, T az, T aw) :
        x(ax),
        y(ay),
        z(az),
        w(aw)
    {
    }

    template<typename T>
    Vector4<T>::Vector4(const Vector4& para) {
        this->x = para.x;
        this->y = para.y;
        this->z = para.z;
        this->w = para.w;
    }

    template<typename T>
    MathLab::Vector4<T>::Vector4(const CrossPlatform::SColorRGBA& rgba)
    {
        this->x = rgba.GetR()/255.0f;
        this->y = rgba.GetG()/255.0f;
        this->z = rgba.GetB()/255.0f;
        this->w = rgba.GetA()/255.0f;
    }

    template<typename T>
    Vector4<T>::~Vector4() {

    }

    template<typename T>
    Vector4<T>& Vector4<T>::operator+=(const Vector4& rvalue)
    {
        this->x += rvalue.x;
        this->y += rvalue.y;
        this->z += rvalue.z;
        this->w += rvalue.w;
        return *this;
    }

    template<typename T>
    Vector4<T>& Vector4<T>::operator-=(const Vector4& rvalue)
    {
        this->x -= rvalue.x;
        this->y -= rvalue.y;
        this->z -= rvalue.z;
        this->w -= rvalue.w;
        return *this;
    }

    template<typename T>
    Vector4<T>& MathLab::Vector4<T>::operator*=(T rvalue)
    {
        this->x *= rvalue;
        this->y *= rvalue;
        this->z *= rvalue;
        this->w *= rvalue;
        return *this;
    }

    template<typename T>
    Vector4<T>& MathLab::Vector4<T>::operator*(T rvalue)const
    {
        Vector4<T> result(*this);
        result *= rvalue;
        return result;
    }

    template<typename T>
    Vector4<T>& MathLab::Vector4<T>::operator/=(T rvalue)
    {
        this->x /= rvalue;
        this->y /= rvalue;
        this->z /= rvalue;
        this->w /= rvalue;
        return *this;
    }

    template<typename T>
    Vector4<T>& MathLab::Vector4<T>::operator/(T rvalue)const
    {
        Vector4f result(*this);
        result /= rvalue;
        return result;
    }

    template<typename T>
    T Vector4<T>::Dot(const Vector4& vec) const
    {
        return this->x * vec.x + this->y * vec.y + this->z * vec.z;
    }

    template<typename T>
    Vector4<T> Vector4<T>::Cross(const Vector4& vec) const
    {
        Vector4 result(
            this->y*vec.z - this->z*vec.y,
            this->z*vec.x - this->x*vec.z,
            this->x*vec.y - this->y*vec.x);
        return result;
    }

    template<typename T>
    bool MathLab::Vector4<T>::IsNormalized() const
    {
        return DoubleMagnitude() == 1;
    }

    template<typename T>
    Vector4<T> Vector4<T>::Normalize() const
    {
        Vector4<T> temp(*this);
        temp.Normalized();
        return temp;
    }

    template<typename T>
    void MathLab::Vector4<T>::Normalized()
    {
        T magnitide = Magnitude();
        assert(magnitide != 0);
        x = x / magnitide;
        y = y / magnitide;
        z = z / magnitide;
    }

    template<typename T>
    T Vector4<T>::Magnitude() const
    {
        return sqrt(DoubleMagnitude());
    }

    template<typename T>
    T Vector4<T>::DoubleMagnitude() const
    {
        return this->Dot(*this);
    }

    template<typename T>
    CrossPlatform::SColorRGBA MathLab::Vector4<T>::ToColor() const
    {
        return CrossPlatform::SColorRGBA(x*255, y*255, z*255, w*255);
    }

    template<typename T>
    Vector4<T> operator+(const Vector4<T>& v1, const Vector4<T>& v2) {
        Vector4<T> vector(v1);
        vector += v2;
        return  vector;
    }

    template<typename T>
    Vector4<T> operator-(const Vector4<T>& v1, const Vector4<T>& v2) {
        Vector4<T> vector(v1);
        vector -= v2;
        return  vector;
    }

    template<typename T>
    void SwapVector(Vector4<T>* vectorA, Vector4<T>* vectorB) {
        Vector4 temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;
        vectorA->z = vectorB->z;
        vectorA->w = vectorB->w;

        vectorB->x = temp.x;
        vectorB->y = temp.y;
        vectorB->z = temp.z;
        vectorB->w = temp.w;

    }

    template<typename T>
    bool LessY(const Vector4<T>& origin, const Vector4<T>& compare) {
        return origin.y < compare.y;
    }

    template<typename T>
    Vector4<T> operator*(const T& v1, const Vector4<T>& value)
    {
        return value * v1;
    }

}
