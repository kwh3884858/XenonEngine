//  Vector3.h
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright © 2018 whkong. All rights reserved.
#pragma once

#include <stdio.h>
namespace MathLab {

    typedef     Vector3<int>    Vector3i;

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

        Vector3();
        Vector3(T ax, T ay, T az);
        Vector3(const Vector3&);
        ~Vector3();

        void Swap();

    };

    template<typename T>
    Vector3 operator+(const Vector3& v1, const Vector3& v2);
    template<typename T>
    Vector3 operator-(const Vector3& v1, const Vector3& v2);

    template<typename T>
    void SwapVector(Vector3* vectorA, Vector3* vectorB);
    template<typename T>
    bool LessY(const Vector3& origin, const Vector3& compare);
    //template<typename T>
    //void Exchange(Vector3*const a, Vector3*const b);

    template<typename T>
    Vector3<T>::Vector3() :
        x(0),
        y(0),
        z(0)
    {
        x = 0;
        y = 0;
        z = 0;
    }

    template<typename T>
    Vector3<T>::Vector3(T ax, T ay, T az) {
        x = ax;
        y = ay;
        z = az;
    }

    template<typename T>
    Vector3<T>::Vector3(const Vector3& para) {
        this->x = para.x;
        this->y = para.y;
        this->z = para.z;
    }

    template<typename T>
    Vector3<T>::~Vector3() {

    }

    template<typename T>
    void Vector3<T>::Swap() {
        T temp = x;
        x = y;
        y = temp;
    }

    template<typename T>
    Vector3& Vector3<T>::operator+=(const Vector3& rvalue)
    {
        this->x += rvalue.x;
        this->y += rvalue.y;
        this->z += rvalue.z;
        return *this;
    }

    template<typename T>
    Vector3& Vector3<T>::operator-=(const Vector3& rvalue)
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
    Vector3 operator+(const Vector3& v1, const Vector3& v2) {
        Vector3 vector(v1);
        vector += v2;
        return  vector;
    }

    template<typename T>
    Vector3 operator-(const Vector3& v1, const Vector3& v2) {
        Vector3 vector(v1);
        vector -= v2;
        return  vector;
    }

    template<typename T>
    void SwapVector(Vector3* vectorA, Vector3* vectorB) {
        Vector3 temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;
        vectorA->z = vectorB->z;

        vectorB->x = temp.x;
        vectorB->y = temp.y;
        vectorB->z = temp.z;

    }

    template<typename T>
    bool LessY(const Vector3& origin, const Vector3& compare) {
        return origin.y < compare.y;
    }

    //template<typename T>
    //void Exchange(Vector3*const a, Vector3*const b) {
    //    Vector3 temp = *a;
    //    a->x = b->x;
    //    a->y = b->y;
    //    a->z = b->z;
    //    b->x = temp.x;
    //    b->y = temp.y;
    //    b->z = temp.z;
    //}

}
