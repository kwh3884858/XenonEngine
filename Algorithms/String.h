//  String.h
//  XenonEngine
//
//  Created by whkong on 2021-4-24.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "Algorithms/Vector.h"
namespace Algorithm
{

    template<typename T>
    class StringBase
    {
    public:
        StringBase();
        StringBase(const StringBase& value);
        StringBase(const T* value, unsigned int size);
        ~StringBase();

        T operator[](int index);

        void Add(T value);
        void Count()const;

    private:
        Vector<T> m_string;
    };

    template<typename T>
    StringBase<T>::StringBase()
    {
    }

    template<typename T>
    StringBase<T>::StringBase(const StringBase& value):
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
    T Algorithm::StringBase<T>::operator[](int index)
    {
        return m_string[index];
    }

    template<typename T>
    void Algorithm::StringBase<T>::Add(T value)
    {
        m_string.Add(value);
    }

    template<typename T>
    void Algorithm::StringBase<T>::Count() const
    {
        return m_string.Count();
    }

    typedef StringBase<char> String;

}