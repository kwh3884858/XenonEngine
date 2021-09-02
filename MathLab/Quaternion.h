#pragma once
#include <cmath>
#include "MathLib.h"
#include "Vector3f.h"

namespace MathLab {
	template<typename T>
	struct Quaternion;

	typedef     Quaternion<float>      Quaternionf;

    template<typename T>
    class Quaternion
    {
    public:
        friend Quaternion operator*(const Quaternion& quaternion, float scale);
        friend Quaternion operator*(float scale, const Quaternion& quaternion);

        Quaternion();
        Quaternion(float w, float x, float y, float z);
        Quaternion(const Vector3f& value);
        Quaternion(const Vector3<T>& normal, T degree);
        Quaternion(const Vector3<T>& Eular);

        Quaternion operator=(const Quaternion& value);
        Quaternion operator+(const Quaternion& value);
        Quaternion operator-(const Quaternion& value);
        Quaternion operator*(const Quaternion& value);

        T Magnitude()const;
        T MagnitudeSquad()const;

        Quaternion ToConjugate()const;
    protected:
    private:
        T m_w;
        T m_x;
        T m_y;
        T m_z;
    };

    template<typename T>
    Quaternion<T> operator*(const Quaternion<T>& quaternion, float scale);
    template<typename T>
    Quaternion<T> operator*(float scale, const Quaternion<T>& quaternion);


    template<typename T>
    MathLab::Quaternion<T>::Quaternion():
        m_w(0),
        m_x(0),
        m_y(0),
        m_z(0)
    {
    }

    template<typename T>
    MathLab::Quaternion<T>::Quaternion(float w, float x, float y, float z):
        m_w(w),
        m_x(x),
        m_y(y),
        m_z(z)
    {
    }

    template<typename T>
    MathLab::Quaternion<T>::Quaternion(const Quaternion& value)
    {
        m_w = value.m_w;
        m_x = value.m_x;
        m_y = value.m_y;
        m_z = value.m_z;
    }

    template<typename T>
    MathLab::Quaternion<T>::Quaternion(const Vector3<T>& normal, T degree)
    {
        float halfTheta = DegreeToRadians(degree);
        m_w = cos(halfTheta / 2);
        m_x = normal.x * sin(halfTheta / 2);
        m_y = normal.y * sin(halfTheta / 2);
        m_z = normal.z * sin(halfTheta / 2);
    }

    template<typename T>
    MathLab::Quaternion<T>::Quaternion(const Vector3<T>& Eular)
    {
        float halfSinX = sin(Eular.x) / 2;
        float halfSinY = sin(Eular.y) / 2;
        float halfSinZ = sin(Eular.z) / 2;

        float halfCosX = cos(Eular.x) / 2;
        float halfCosY = cos(Eular.y) / 2;
        float halfCosZ = cos(Eular.z) / 2;

        m_w = halfCosZ * halfCosY * halfCosX + halfSinZ * halfSinY * halfSinX;
        m_x = halfCosZ * halfCosY * halfSinX - halfSinZ * halfSinY * halfCosX;
        m_y = halfCosZ * halfSinY * halfCosX + halfSinZ * halfCosY * halfSinX;
        m_z = halfSinZ * halfCosY * halfSinX - halfCosZ * halfSinY * halfSinX;
    }

    template<typename T>
	Quaternion<T> MathLab::Quaternion<T>::operator=(const Quaternion& value)
    {
        if (this == &value)
        {
            return *this;
        }
        Quaternion result;
        result.m_w = value.m_w;
        result.m_x = value.m_x;
        result.m_y = value.m_y;
        result.m_z = value.m_z;
    }

    template<typename T>
	Quaternion<T> MathLab::Quaternion<T>::operator+(const Quaternion& value)
    {
        m_w += value.m_w;
        m_x += value.m_x;
        m_y += value.m_y;
        m_z += value.m_z;
    }

    template<typename T>
	Quaternion<T> MathLab::Quaternion<T>::operator-(const Quaternion& value)
    {
        m_w -= value.m_w;
        m_x -= value.m_x;
        m_y -= value.m_y;
        m_z -= value.m_z;
    }

    template<typename T>
	Quaternion<T> MathLab::Quaternion<T>::operator*(const Quaternion& value)
    {
        m_w = m_w * value.m_w - m_x * value.m_x - m_y * value.m_y - m_z * value.m_z;
        m_x = m_w * value.m_x + m_x * value.m_w + m_y * value.m_z - m_z * value.m_y;
        m_y = m_w * value.m_y + m_y * value.m_w + m_z * value.m_x - m_x * value.m_z;
        m_z = m_w * value.m_z + m_z * value.m_w + m_x * value.m_y - m_y * value.m_x;
    }

    template<typename T>
    T MathLab::Quaternion<T>::Magnitude() const
    {
        return sqrt(MagnitudeSquad());
    }

    template<typename T>
    T MathLab::Quaternion<T>::MagnitudeSquad() const
    {
        return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
    }

    template<typename T>
	Quaternion<T> MathLab::Quaternion<T>::ToConjugate() const
    {
        Quaternion result(this->m_w, -this->m_x, -this->m_y, -this->m_z);
        return result;
    }

    template<typename T>
	Quaternion<T> operator*(const Quaternion<T>& quaternion, float scale)
    {
        Quaternion result(quaternion.m_w*scale, quaternion.m_x *scale, quaternion.m_y*scale, quaternion.m_z*scale);
        return result;
    }

    template<typename T>
	Quaternion<T> operator*(float scale, const Quaternion<T>& quaternion)
    {
        return quaternion * scale;
    }

}