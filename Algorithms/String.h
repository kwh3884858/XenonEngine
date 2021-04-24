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

        void Add(T value);

    private:
        Vector<T> m_string;
    };

    template<typename T>
    void Algorithm::StringBase<T>::Add(T value)
    {
        m_string.Add(value);
    }

    template<typename T>
    StringBase<T>::StringBase()
    {
    }

    template<typename T>
    StringBase<T>::StringBase(const StringBase& value)
    {
        m_string(value);
    }

    template<typename T>
    Algorithm::StringBase<T>::StringBase(const T* value, unsigned int size)
    {
        m_string.Replace(value, size);
    }

    typedef StringBase<char> String;
}