//  String.h
//  XenonEngine
//
//  Created by whkong on 2021-4-24.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include <cstdlib>
#include "Algorithms/Vector.h"
namespace Algorithm
{
    template<typename T>
    class StringBase;

    template<typename T>
    bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs);

    template<typename T>
    class StringBase
    {
    public:
        template<typename T>
        friend bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs);

        StringBase();
        StringBase(const StringBase& value);
        StringBase(const T* value, unsigned int size);
        ~StringBase();

        T operator[](int index) const;
        T operator[](unsigned int index) const;
        StringBase<T>& operator=(const StringBase<T>& rhs);
        StringBase<T>& operator=(const T* rhs);
        bool operator==(const T* rhs)const;

        void Add(T value);
        int Count()const;
        int ToInt()const;
        float ToFloat()const;
        char ToChar()const;
        void Clear();
    private:
        Vector<T> m_string;
    };


    template<typename T>
    StringBase<T>::StringBase()
    {
    }

    template<typename T>
    StringBase<T>::StringBase(const StringBase& value) :
        m_string(value.m_string)
    {
    }

    template<typename T>
    Algorithm::StringBase<T>::StringBase(const T* value, unsigned int size)
    {
        m_string.Replace(value, size);
    }

    template<typename T>
    Algorithm::StringBase<T>::~StringBase()
    {
    }

    template<typename T>
    T Algorithm::StringBase<T>::operator[](int index)const
    {
        return m_string[index];
    }

    template<typename T>
    inline T StringBase<T>::operator[](unsigned int index)const
    {
        return m_string[index];
    }

    template<typename T>
    inline StringBase<T>& StringBase<T>::operator=(const StringBase<T>& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        this->m_string = rhs.m_string;
        return *this;
    }

    template<typename T>
    inline StringBase<T>& StringBase<T>::operator=(const T* rhs)
    {
        int index = 0;
        while (rhs[index] != '\0')
        {
            m_string.Add(rhs[index]);
        }
        return *this;
    }

    template<typename T>
    inline bool StringBase<T>::operator==(const T* rhs) const
    {
        for ( int i = 0; i < Count(); i++)
        {
            if (m_string[i] != rhs[i])
            {
                return false;
            }
        }
        if (rhs[Count() + 1] == '\0')
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    template<typename T>
    void Algorithm::StringBase<T>::Add(T value)
    {
        m_string.Add(value);
    }

    template<typename T>
     int Algorithm::StringBase<T>::Count() const
    {
        return m_string.Count();
    }

    template<typename T>
    int Algorithm::StringBase<T>::ToInt() const
    {
         int size = m_string.Count() + 1;
        T* content = new T[size];
        memcpy(content, m_string.begin(), m_string.Count());
        content[size] = '\0';
        int result = atoi(content);
        return result;
    }


    template<typename T>
    float Algorithm::StringBase<T>::ToFloat() const
    {
        int size = m_string.Count() + 1;
        T* content = new T[size];
        memcpy(content, m_string.begin(), m_string.Count());
        content[size] = '\0';
        float result = atof(content);
        return result;
    }

    template<typename T>
    inline char StringBase<T>::ToChar() const
    {
        if (Count() < 1)
        {
            return 0;
        }
        return (*this)[0];
    }

    template<typename T>
    inline void StringBase<T>::Clear()
    {
        m_string.Clear();
    }

    template<typename T>
    bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs)
    {
        if (lhs.Count() != rhs.Count())
        {
            return false;
        }
        for (int i = 0; i < lhs.Count(); i++)
        {
            if (lhs[i] != rhs[i])
            {
                return false;
            }
        }
        return true;
    }

    typedef StringBase<char> String;

}