//  Vector2f.h
//  XenonEngine
//
//  Created by whkong on 2021-4-14.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#ifndef VectorStruct_h
#define VectorStruct_h

//#include <stdio.h>
namespace MathLab {
    typedef Vector2<float> Vector2f;

    template<typename T>
    struct Vector2 final {
        T x;
        T y;

    public:
        friend void SwapVector(Vector2* vectorA, Vector2* vectorB);
        friend bool LessY(const Vector2& origin, const Vector2& compare);
        friend void Exchange(Vector2*const a, Vector2*const b);

        friend Vector2 operator+(const Vector2& v1, const Vector2& v2);
        friend Vector2 operator-(const Vector2& v1, const Vector2& v2);

        Vector2();
        Vector2(T ax, T ay);
        Vector2(const Vector2&);
        ~Vector2();

        void Swap();
    };


    template<typename T>
    Vector2 operator+(const Vector2& v1, const Vector2& v2);
    template<typename T>
    Vector2 operator-(const Vector2& v1, const Vector2& v2);

    template<typename T>
    void SwapVector(Vector2* vectorA, Vector2* vectorB);
    template<typename T>
    bool LessY(const Vector2& origin, const Vector2& compare);
    template<typename T>
    void Exchange(Vector2*const a, Vector2*const b);

    template<typename T>
    Vector2<T>::Vector2() {
        x = 0;
        y = 0;
    }

    template<typename T>
    Vector2<T>::Vector2(int ax, int ay) {
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
    void Vector2<T>::Swap() {
        int temp = x;
        x = y;
        y = temp;
    }

    template<typename T>
    Vector2 operator+(const Vector2& v1, const Vector2& v2) {
        Vector2 vector;
        vector.x = v1.x + v2.x;
        vector.y = v1.y + v2.y;
        return  vector;
    }

    template<typename T>
    Vector2 operator-(const Vector2& v1, const Vector2& v2) {
        Vector2 vector;
        vector.x = v1.x - v2.x;
        vector.y = v1.y - v2.y;
        return  vector;
    }

    template<typename T>
    void SwapVector(Vector2* vectorA, Vector2* vectorB) {
        Vector2 temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;

        vectorB->x = temp.x;
        vectorB->y = temp.y;
    }

    template<typename T>
    bool LessY(const Vector2& origin, const Vector2& compare) {
        return origin.y < compare.y;
    }

    template<typename T>
    void Exchange(Vector2*const a, Vector2*const b) {
        Vector2 temp = *a;
        a->x = b->x;
        a->y = b->y;
        b->x = temp.x;
        b->y = temp.y;
    }

}
#endif /* VectorStruct_h */
