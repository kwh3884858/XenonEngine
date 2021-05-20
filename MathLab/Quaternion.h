#pragma once
namespace MathLab {
    template<typename T>
    class Quaternion
    {
    public:
        Quaternion();
        Quaternion(const Quaternion& value);

        Quaternion operator=(const Quaternion& value);
        Quaternion operator+(const Quaternion& value);
        Quaternion operator-(const Quaternion& value);
    protected:
    private:
        T m_w;
        T m_x;
        T m_y;
        T m_z;
    };

    template<typename T>
    MathLab::Quaternion<T>::Quaternion() :
        m_q(0),
        m_x(0),
        m_y(0),
        m_z(0),
    {
    }

    template<typename T>
    MathLab::Quaternion<T>::Quaternion(const Quaternion& value)
    {
        m_q = value.m_w;
        m_x = value.m_x;
        m_y = value.m_y;
        m_z = value.m_z;
    }

    template<typename T>
    Quaternion MathLab::Quaternion<T>::operator=(const Quaternion& value)
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

}