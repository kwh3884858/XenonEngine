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
        void ToInt()const;

    private:
        Vector<T> m_string;
    };

    template<typename T>
    bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs);

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

    template<typename T>
    void Algorithm::StringBase<T>::ToInt() const
    {
        int size = m_string.Count() + 1;
        T* content = new content[size];
        memcpy(content, m_string.begin(), m_string.Count());
        content[size] = '\0';
        int result = atoi(content);
        return result;
    }

    template<typename T>
    bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs)
    {
        if (lhs.Count() != rhs.Count())
        {
            return false;
        }
        for (int i = 0 ; i < lhs.Count();i++)
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