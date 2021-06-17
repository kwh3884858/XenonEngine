//  String.h
//  XenonEngine
//
//  Created by whkong on 2021-4-24.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include <cstdlib>
#include "Algorithms/Vector.h"
#include "Algorithms/DeterministicFiniteAutomaton.h"

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
        StringBase(const T* value);
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

        int Find(const StringBase& subString);
        void Append(const StringBase& subString);
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
    Algorithm::StringBase<T>::StringBase(const T* value)
    {
        int index = 0;
        while (value[index] != '\0')
        {
            m_string.Add(value[index]);
        }
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


    template<typename T>
    int Algorithm::StringBase<T>::Find(const StringBase& subString)
    {
        int character[265];
        int nonCharacterCount = 0;

        //Count character occurrences
        memset(character, 0, sizeof(character));
        for (int i = 0; i < subString.Count(); i++) {
            int index = subString[i];
            character[index]++;
        }
        for (int i = 0; i < 265; i++) {
            if (character[i] != 0) {
                nonCharacterCount++;
            }
        }

        DeterministicFiniteAutomaton* pOutDFA = new DeterministicFiniteAutomaton;
        pOutDFA->m_characterCount = nonCharacterCount;
        pOutDFA->m_countentlength = subString.Count();

        pOutDFA->m_next = new int[pOutDFA->m_characterCount * pOutDFA->m_countentlength];
        memset(pOutDFA->m_next, 0, pOutDFA->m_characterCount * pOutDFA->m_countentlength * sizeof(T));
        pOutDFA->m_character = new char[pOutDFA->m_characterCount];

        nonCharacterCount = 0;
        for (int i = 0; i < 265; i++) {
            if (character[i] != 0) {
                pOutDFA->m_character[nonCharacterCount++] = i;
            }
        }

        int X = 0;
        pOutDFA->Set(pOutDFA->GetCharacterPos(subString[0]), 0, 1);
        for (int i = 1; i < pOutDFA->m_countentlength; i++) {
            for (int j = 0; j < pOutDFA->m_characterCount; j++) {
                pOutDFA->Set(j, i, pOutDFA->Get(j, X));
            }
            int charPos = pOutDFA->GetCharacterPos(subString[i]);

            pOutDFA->Set(charPos, i, i + 1);

            X = pOutDFA->Get(charPos, X);
        }

        // Find content
        int j = 0;
        for (int i = 0; i < Count(); i++) {
            j = pOutDFA->Get(pOutDFA->GetCharacterPos(m_string[i]), j);

            if (j == subString.Count()) {
                delete pOutDFA;
                return i + 1 - subString.Count();
            }
        }
        delete pOutDFA;
        return -1;
    }

    template<typename T>
    void Algorithm::StringBase<T>::Append(const StringBase& subString)
    {
        for (int i = 0; i < subString.Count(); i ++)
        {
            m_string.Add(subString[i]);
        }
    }


    typedef StringBase<char> String;

}