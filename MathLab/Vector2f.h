//  Vector2.h
//  XenonEngine
//
//  Created by whkong on 2021-4-14.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#ifndef VectorStruct_h
#define VectorStruct_h

//#include <stdio.h>
namespace MathLab {

    template<typename T>
    struct Vector2;

    template<typename T>
    Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2);
    template<typename T>
    Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2);

    template<typename T>
    void SwapVector(Vector2<T>* vectorA, Vector2<T>* vectorB);
    template<typename T>
    void SwapVector(Vector2<T>& vectorA, Vector2<T>& vectorB);
    template<typename T>
    bool LessY(const Vector2<T>& origin, const Vector2<T>& compare);
    template<typename T>
    void Exchange(Vector2<T>*const a, Vector2<T>*const b);

    template<typename T>
    struct Vector2 final {
        T x;
        T y;

    public:
        friend void SwapVector<T>(Vector2<T>* vectorA, Vector2<T>* vectorB);
        friend void SwapVector<T>(Vector2<T>& vectorA, Vector2<T>& vectorB);
        friend bool LessY<T>(const Vector2<T>& origin, const Vector2<T>& compare);
        friend void Exchange<T>(Vector2<T>*const a, Vector2<T>*const b);
        friend Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2);
        friend Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2);

        Vector2& operator=(const Vector2& value);

        Vector2();
        Vector2(T ax, T ay);
        Vector2(const Vector2&);
        ~Vector2();

        void Swap();
    };

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
    void Vector2<T>::Swap() {
        T temp = x;
        x = y;
        y = temp;
    }

    template<typename T>
    Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2) {
        Vector2 vector;
        vector.x = v1.x + v2.x;
        vector.y = v1.y + v2.y;
        return  vector;
    }

    template<typename T>
    Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2) {
        Vector2<T> vector;
        vector.x = v1.x - v2.x;
        vector.y = v1.y - v2.y;
        return  vector;
    }

    template<typename T>
    void SwapVector(Vector2<T>* vectorA, Vector2<T>* vectorB) {
        Vector2<T> temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;

        vectorB->x = temp.x;
        vectorB->y = temp.y;
    }

    template<typename T>
    void SwapVector(Vector2<T>& vectorA, Vector2<T>& vectorB)
    {
        Vector2<T> temp (vectorA);
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;

        vectorB->x = temp.x;
        vectorB->y = temp.y;
    }

    template<typename T>
    bool LessY(const Vector2<T>& origin, const Vector2<T>& compare) {
        return origin.y < compare.y;
    }

    template<typename T>
    void Exchange(Vector2<T>*const a, Vector2<T>*const b) {
        Vector2<T> temp = *a;
        a->x = b->x;
        a->y = b->y;
        b->x = temp.x;
        b->y = temp.y;
    }

    typedef Vector2<float> Vector2f;

    MathLab::Vector2f m_position;

}
#endif /* VectorStruct_h */
