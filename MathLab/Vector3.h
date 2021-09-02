//  Vector3.h
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "MathLab/Vector2.h"
namespace MathLab {
    template<typename T>
    struct Vector3;

    typedef     Vector3<int>        Vector3i;
    typedef     Vector3<float>      Vector3f;

    template<typename T>
    struct Vector3 final {
        T x;
        T y;
        T z;

    public:
        friend void SwapVector(Vector3* vectorA, Vector3* vectorB);
        friend bool LessY(const Vector3& origin, const Vector3& compare);
        //friend void Exchange(Vector3*const a, Vector3*const b);

        friend Vector3 operator+(const Vector3& v1, const Vector3& v2);
        friend Vector3 operator-(const Vector3& v1, const Vector3& v2);

        Vector3& operator+=(const Vector3& rvalue);
        Vector3& operator-=(const Vector3& rvalue);

        T dot(const Vector3& vec)const;
        Vector3 Cross(const Vector3& vec)const;

        Vector3();
        Vector3(T ax, T ay, T az);
        Vector3(const Vector3& vec);
        Vector3(const Vector2<T>& vec);
        ~Vector3();

        void Swap();
        Vector3 Normalize()const;
        T Magnitude()const;
        T DoubleMagnitude()const;
    };

    template<typename T>
    Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2);
    template<typename T>
    Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2);

    template<typename T>
    void SwapVector(Vector3<T>* vectorA, Vector3<T>* vectorB);
    template<typename T>
    bool LessY(const Vector3<T>& origin, const Vector3<T>& compare);
    //template<typename T>
    //void Exchange(Vector3*const a, Vector3*const b);

    template<typename T>
    Vector3<T>::Vector3() :
        x(0),
        y(0),
        z(0)
    {
    }

    template<typename T>
    Vector3<T>::Vector3(T ax, T ay, T az) :
        x(ax),
        y(ay),
        z(az)
    {
    }

    template<typename T>
    Vector3<T>::Vector3(const Vector3& para) {
        this->x = para.x;
        this->y = para.y;
        this->z = para.z;
    }

    template<typename T>
    Vector3<T>::Vector3(const Vector2<T>& vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = 0;
    }

    template<typename T>
    Vector3<T>::~Vector3() {

    }

    template<typename T>
    Vector3<T>& Vector3<T>::operator+=(const Vector3& rvalue)
    {
        this->x += rvalue.x;
        this->y += rvalue.y;
        this->z += rvalue.z;
        return *this;
    }

    template<typename T>
    Vector3<T>& Vector3<T>::operator-=(const Vector3& rvalue)
    {
        this->x -= rvalue.x;
        this->y -= rvalue.y;
        this->z -= rvalue.z;
        return *this;
    }

    template<typename T>
    T Vector3<T>::dot(const Vector3& vec) const
    {
        return this->x * vec.x + this->y * vec.y + this->z * vec.z;
    }

    template<typename T>
    Vector3<T> Vector3<T>::Cross(const Vector3& vec) const
    {
        Vector3 result(this->y * vec.z - this->z * vec.z,
            this->z*vec.x - this->x*vec.z,
            this->x*vec.y - this->y*vec.x);
        return result;
    }

    template<typename T>
    void Vector3<T>::Swap() {
        T temp = x;
        x = y;
        y = temp;
    }

    template<typename T>
    Vector3<T> Vector3<T>::Normalize() const
    {
        T magnitide = Magnitude();
        Vector3<T> temp;
        temp->x = this->x / magnitide;
        temp->y = this->y / magnitide;
        temp->z = this->z / magnitide;
        return temp;
    }

    template<typename T>
    T Vector3<T>::Magnitude() const
    {
        return sqrt(DoubleMagnitude());
    }

    template<typename T>
    T Vector3<T>::DoubleMagnitude() const
    {
        return this->dot(*this);
    }

    template<typename T>
    Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2) {
        Vector3 vector(v1);
        vector += v2;
        return  vector;
    }

    template<typename T>
    Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2) {
        Vector3 vector(v1);
        vector -= v2;
        return  vector;
    }

    template<typename T>
    void SwapVector(Vector3<T>* vectorA, Vector3<T>* vectorB) {
        Vector3 temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;
        vectorA->z = vectorB->z;

        vectorB->x = temp.x;
        vectorB->y = temp.y;
        vectorB->z = temp.z;

    }

    template<typename T>
    bool LessY(const Vector3<T>& origin, const Vector3<T>& compare) {
        return origin.y < compare.y;
    }

}
