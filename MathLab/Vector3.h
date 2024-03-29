//  Vector3.h
//  MacWindow
//
//  Created by whkong on 2021-2-28.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "MathLab/Vector2.h"
#include <cassert>
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
		static Vector3 Zero;

        friend void SwapVector(Vector3* vectorA, Vector3* vectorB);
        friend bool LessY(const Vector3& origin, const Vector3& compare);
        //friend void Exchange(Vector3*const a, Vector3*const b);

        Vector3& operator+=(const Vector3& rvalue);
        Vector3& operator-=(const Vector3& rvalue);
        Vector3 operator-();
        Vector3& operator*=(T rvalue);
        Vector3& operator*(T rvalue);
        Vector3& operator/=(T rvalue);
        Vector3& operator/(T rvalue);
		bool operator==(const Vector3& that)const;
		bool operator!=(const Vector3& that)const;

        T Dot(const Vector3& vec)const;
        Vector3 Cross(const Vector3& vec)const;

        Vector3();
        Vector3(T ax, T ay, T az);
        Vector3(const Vector3& vec);
        Vector3(const Vector2<T>& vec);
        ~Vector3();

        void Swap();
        bool IsNormalized()const;
        Vector3 Normalize()const;
        void Normalized();
        T Magnitude()const;
        T DoubleMagnitude()const;
    };

	template<typename T>
	__declspec(selectany) Vector3<T> Vector3<T>::Zero = Vector3f(0, 0, 0);

    template<typename T>
    Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2);
    template<typename T>
    Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2);

    template<typename T>
    void SwapVector(Vector3<T>* vectorA, Vector3<T>* vectorB);
    template<typename T>
    bool LessY(const Vector3<T>& origin, const Vector3<T>& compare);
    template<typename T>
    Vector3<T> operator*(const T& v1, const Vector3<T>& value);
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
    Vector3<T> MathLab::Vector3<T>::operator-()
    {
        Vector3<T> result(*this);
        result.x = -result.x;
        result.y = -result.y;
        result.z = -result.z;
        return result;
    }

    template<typename T>
    Vector3<T>& MathLab::Vector3<T>::operator*=(T rvalue)
    {
        this->x -= rvalue.x;
        this->y -= rvalue.y;
        this->z -= rvalue.z;
        return *this;
    }

    template<typename T>
    Vector3<T>& MathLab::Vector3<T>::operator*(T rvalue)
    {
        Vector3<T> result(*this);
        result *= rvalue;
        return result;
    }

    template<typename T>
    Vector3<T>& MathLab::Vector3<T>::operator/=(T rvalue)
    {
        this->x /= rvalue.x;
        this->y /= rvalue.y;
        this->z /= rvalue.z;
        return *this;
    }

    template<typename T>
    Vector3<T>& MathLab::Vector3<T>::operator/(T rvalue)
    {
        Vector3f result(*this);
        result /= rvalue;
        return result;
    }

	template<typename T>
	bool MathLab::Vector3<T>::operator==(const Vector3& that) const
	{
		if (this == &that)
		{
			return true;
		}
		if (MathLab::Abs(x - that.x) > EPSILON ||
			MathLab::Abs(y - that.y) > EPSILON ||
			MathLab::Abs(z - that.z) > EPSILON)
		{
			return false;
		}
		return true;
	}

	template<typename T>
	bool MathLab::Vector3<T>::operator!=(const Vector3& that) const
	{
		return !((*this) == that);
	}

    template<typename T>
    T Vector3<T>::Dot(const Vector3& vec) const
    {
        return this->x * vec.x + this->y * vec.y + this->z * vec.z;
    }

    template<typename T>
    Vector3<T> Vector3<T>::Cross(const Vector3& vec) const
    {
        Vector3 result(
            this->y*vec.z - this->z*vec.y,
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
    bool MathLab::Vector3<T>::IsNormalized() const
    {
        return DoubleMagnitude() == 1;
    }

    template<typename T>
    Vector3<T> Vector3<T>::Normalize() const
    {
        Vector3<T> temp(*this);
        temp.Normalized();
        return temp;
    }

    template<typename T>
    void MathLab::Vector3<T>::Normalized()
    {
        T magnitide = Magnitude();
        assert(magnitide != 0);
        x = x / magnitide;
        y = y / magnitide;
        z = z / magnitide;
    }

    template<typename T>
    T Vector3<T>::Magnitude() const
    {
        return (T) sqrt(DoubleMagnitude());
    }

    template<typename T>
    T Vector3<T>::DoubleMagnitude() const
    {
        return this->Dot(*this);
    }

    template<typename T>
    Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2) {
        Vector3<T> vector(v1);
        vector += v2;
        return  vector;
    }

    template<typename T>
    Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2) {
        Vector3<T> vector(v1);
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

    template<typename T>
    Vector3<T> operator*(const T& v1, const Vector3<T>& value)
    {
        return value * v1;
    }

}
