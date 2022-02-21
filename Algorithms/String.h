//  String.h
//  XenonEngine
//
//  Created by whkong on 2021-4-24.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include <cstdlib>
#include "Algorithms/Vector.h"
#include "Algorithms/DeterministicFiniteAutomaton.h"
#include "Algorithms/AlgorithmMacro.h"

namespace Algorithm
{
    template<typename T>
    class StringBase;

    typedef StringBase<char> String;

    template<typename T>
    bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs);
	template<typename T>
	bool Swap(StringBase<T>& lhs, StringBase<T>& rhs);
    //template<typename T>
    //StringBase<T> operator+(const StringBase<T> lhs, const StringBase<T> rhs);

    template<typename T>
    class StringBase
    {
    public:
        static const int INVALID_VALUE;
        static const StringBase<char> INVALID_STRING;

        template<typename T>
        friend bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs);
        StringBase();
        StringBase(const StringBase& value);
        StringBase(const T* value, int size);
        StringBase(const T* value);
        StringBase(T value);
        ~StringBase();

        T& operator[](int index);
        const T& operator[](int index) const;
        T& operator[](unsigned int index);
        const T& operator[](unsigned int index) const;
        StringBase<T>& operator=(const StringBase<T>& rhs);
        StringBase<T>& operator=(const T* rhs);
        bool operator==(const StringBase<T>& rhs)const;
        bool operator!=(const StringBase<T>& rhs)const;
        StringBase<T> operator+(const StringBase<T> text)const;

        void Add(T value);
        int Count()const;
        int Capacity()const { return m_string.Capacity(); }
        int ToInt()const;
        float ToFloat()const;
        char ToChar()const;
        const char* const CString()const;
        void Clear();
        const T* const Beign()const;

        int IndexOf(const StringBase& subString)const;
        int LastIndexOf(const StringBase& subString)const; 
        int IndexOf(const T& subChar, int startPos = 0)const;
        int LastIndexOf(const T& subChar)const;
        bool Find(const StringBase& subString)const;
        void Append(const StringBase& subString);
        StringBase<T> Substring(int start, int end)const; //[start,end)
        Vector<StringBase<T>> Split(T delimiter) const;
        bool Empty()const { return Count() == 0; }
        void Resize(int length);
    private:
        void CStringlize();

        Vector<T> m_string;
    };

	template<typename T>
	__declspec(selectany) const StringBase<char> Algorithm::StringBase<T>::INVALID_STRING = "Invalid String";

    template<typename T>
    __declspec(selectany) const int Algorithm::StringBase<T>::INVALID_VALUE = -1;;

    template<typename T>
    StringBase<T>::StringBase()
    {
        CStringlize();
    }

    template<typename T>
    StringBase<T>::StringBase(const StringBase& value) :
        m_string(value.m_string)
    {
        CStringlize();
    }

    template<typename T>
    Algorithm::StringBase<T>::StringBase(const T* value, int size)
    {
        m_string.Replace(value, size);
        CStringlize();
    }

    template<typename T>
    Algorithm::StringBase<T>::StringBase(const T* value)
    {
        int index = 0;
        while (value[index] != '\0')
        {
            m_string.Add(value[index]);
            index++;
        }
        CStringlize();
    }

    template<typename T>
    inline StringBase<T>::StringBase(T value)
    {
        m_string.Add(value);
        CStringlize();
    }

    template<typename T>
    Algorithm::StringBase<T>::~StringBase()
    {
    }

    template<typename T>
    inline T& StringBase<T>::operator[](int index)
    {
        return const_cast<T&>(static_cast<const StringBase<T>&>(*this)[index]);
    }

    template<typename T>
    inline const T& Algorithm::StringBase<T>::operator[](int index)const
    {
        assert(index >= 0 && index < m_string.Count());
        return m_string[index];
    }

    template<typename T>
    inline T& StringBase<T>::operator[](unsigned int index)
    {
        return const_cast<T&>(static_cast<const StringBase<T>&>(*this)[index]);
    }

    template<typename T>
    inline const T& StringBase<T>::operator[](unsigned int index)const
    {
        assert(index >= 0 && index < m_string.Count());
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
        CStringlize();
        return *this;
    }

    template<typename T>
    inline StringBase<T>& StringBase<T>::operator=(const T* rhs)
    {
        int index = 0;
        while (rhs[index] != '\0')
        {
            m_string.Add(rhs[index++]);
        }
        CStringlize();
        return *this;
    }

    template<typename T>
    bool Algorithm::StringBase<T>::operator==(const StringBase<T>& rhs)const
    {
        if (Count() != rhs.Count())
        {
            return false;
        }
        for (int i = 0; i < Count(); i++)
        {
            if (m_string[i] != rhs[i])
            {
                return false;
            }
        }
        return true;
    }

    //template<typename T>
    //bool Algorithm::StringBase<T>::operator!=(const T* rhs) const
    //{
    //    return !((*this) == rhs);
    //}

    template<typename T>
    bool Algorithm::StringBase<T>::operator!=(const StringBase<T>& rhs)const
    {
        return !((*this) == rhs);
    }

    template<typename T>
    Algorithm::StringBase<T> Algorithm::StringBase<T>::operator+(const StringBase<T> text) const
    {
        StringBase<T> result(*this);
        result.Append(text);
        return result;
    }

    template<typename T>
    void Algorithm::StringBase<T>::Add(T value)
    {
        m_string.Add(value);
        CStringlize();
    }

    template<typename T>
     int Algorithm::StringBase<T>::Count() const
    {
        return m_string.Count();
    }

    template<typename T>
    int Algorithm::StringBase<T>::ToInt() const
    {
#ifdef XenonEngine_Debug
        for (int i = 0; i < Count(); i++)
        {
            assert((m_string[i] >= '0' && m_string[i] <= '9') == true);
        }
#endif
        int size = m_string.Count() + 1;
        T* content = new T[size];
        memcpy(content, m_string.Begin(), m_string.Count());
        content[size] = '\0';
        int result = atoi(content);
        return result;
    }


    template<typename T>
    float Algorithm::StringBase<T>::ToFloat() const
    {
        int size = m_string.Count() + 1;
        T* content = new T[size];
        memcpy(content, m_string.Begin(), m_string.Count());
        content[m_string.Count()] = '\0';
        float result = (float) atof(content);
        delete content;
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
    const char* const Algorithm::StringBase<T>::CString()const
    {
        return m_string.Begin();
    }

    template<typename T>
    inline void StringBase<T>::Clear()
    {
        m_string.Clear();
    }

    template<typename T>
    inline const T* const StringBase<T>::Beign() const
    {
        return m_string.Begin();
    }

    //template<typename T>
    //StringBase<T>
    //    operator+(const StringBase<T> lhs, const StringBase<T> rhs)
    //{
    //    return lhs + rhs;
    //}

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
	bool Swap(StringBase<T>& lhs, StringBase<T>& rhs)
	{
		StringBase tmp(lhs);
		lhs = rhs;
		rhs = tmp;
        lhs.CStringlize();
        rhs.CStringlize();
		return true;
	}


    template<typename T>
    int Algorithm::StringBase<T>::IndexOf(const StringBase& subString)const
    {
        int character[265];
        int nonCharacterCount = 0;

        //Construct DFA

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

        DeterministicFiniteAutomaton<T>* pOutDFA = new DeterministicFiniteAutomaton<T>;
        pOutDFA->m_characterCount = nonCharacterCount;
        pOutDFA->m_countentlength = subString.Count();

        pOutDFA->m_next = new int[pOutDFA->m_characterCount * pOutDFA->m_countentlength];
        memset(pOutDFA->m_next, 0, pOutDFA->m_characterCount * pOutDFA->m_countentlength * sizeof(int));
        pOutDFA->m_character = new char[pOutDFA->m_characterCount];

        nonCharacterCount = 0;
        for (int i = 0; i < 265; i++) {
            if (character[i] != 0) {
                pOutDFA->m_character[nonCharacterCount++] = static_cast<T>( i );
            }
        }

        const int startPos = 0;
        const int nextPosWhenFirstCharacterIsCorrect = 1;
        pOutDFA->Set(pOutDFA->GetCharacterPos(subString[0]), startPos, nextPosWhenFirstCharacterIsCorrect);
        
        int restartPos = 0;
        for (int currentSubStringIndex = 1; currentSubStringIndex < pOutDFA->m_countentlength; currentSubStringIndex++) {
            for (int characterIndex = 0; characterIndex < pOutDFA->m_characterCount; characterIndex++) {
                pOutDFA->Set(characterIndex, currentSubStringIndex, pOutDFA->Get(characterIndex, restartPos));
            }
            int characterTablePos = pOutDFA->GetCharacterPos(subString[currentSubStringIndex]);

            pOutDFA->Set(characterTablePos, currentSubStringIndex, currentSubStringIndex + 1);

            restartPos = pOutDFA->Get(characterTablePos, restartPos);
        }

        // Find content
        int nextPos = 0;
        for (int i = 0; i < Count(); i++) {
            nextPos = pOutDFA->Get(pOutDFA->GetCharacterPos(m_string[i]), nextPos);

            if (nextPos == subString.Count()) {
                delete pOutDFA;
                return i + 1 - subString.Count();
            }
        }
        delete pOutDFA;
        return INVALID_VALUE;
    }

    template<typename T>
    int Algorithm::StringBase<T>::LastIndexOf(const StringBase& subString) const
    {
        int character[265];
        int nonCharacterCount = 0;

        //Construct DFA

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

        DeterministicFiniteAutomaton<T>* pOutDFA = new DeterministicFiniteAutomaton<T>;
        pOutDFA->m_characterCount = nonCharacterCount;
        pOutDFA->m_countentlength = subString.Count();

        pOutDFA->m_next = new int[pOutDFA->m_characterCount * pOutDFA->m_countentlength];
        memset(pOutDFA->m_next, 0, pOutDFA->m_characterCount * pOutDFA->m_countentlength * sizeof(int));
        pOutDFA->m_character = new char[pOutDFA->m_characterCount];

        nonCharacterCount = 0;
        for (int i = 0; i < 265; i++) {
            if (character[i] != 0) {
                pOutDFA->m_character[nonCharacterCount++] = i;
            }
        }

        const int startPos = 0;
        const int nextPosWhenFirstCharacterIsCorrect = 1;
        pOutDFA->Set(pOutDFA->GetCharacterPos(subString[pOutDFA->m_countentlength -1]), startPos, nextPosWhenFirstCharacterIsCorrect);

        int restartPos = 0;
        for (int currentSubStringIndex = pOutDFA->m_countentlength - 2; currentSubStringIndex > 0; currentSubStringIndex--) {
            for (int character = 0; character < pOutDFA->m_characterCount; character++) {
                pOutDFA->Set(character, currentSubStringIndex, pOutDFA->Get(character, restartPos));
            }
            int characterTablePos = pOutDFA->GetCharacterPos(subString[currentSubStringIndex]);

            pOutDFA->Set(characterTablePos, currentSubStringIndex, currentSubStringIndex + 1);

            restartPos = pOutDFA->Get(characterTablePos, restartPos);
        }

        // Find content
        int nextPos = 0;
        for (int i = Count(); i > 0; i++) {
            nextPos = pOutDFA->Get(pOutDFA->GetCharacterPos(m_string[i]), nextPos);

            if (nextPos == subString.Count()) {
                delete pOutDFA;
                return i + 1;
            }
        }
        delete pOutDFA;
        return INVALID_VALUE;
    }

    template<typename T>
    int Algorithm::StringBase<T>::IndexOf(const T& subChar, int startPos) const
    {
        for (int i = startPos; i < m_string.Count(); i++)
        {
            if (m_string[i] == subChar)
            {
                return i;
            }
        }
        return INVALID_VALUE;
    }

    template<typename T>
    int Algorithm::StringBase<T>::LastIndexOf(const T& subChar) const
    {
        for (int i = m_string.Count() -1; i >= 0 ; i--)
        {
            if (m_string[i] == subChar)
            {
                return i;
            }
        }
        return INVALID_VALUE;
    }

    template<typename T>
    inline bool StringBase<T>::Find(const StringBase& subString)const
    {
        return IndexOf(subString) != INVALID_VALUE;
    }

    template<typename T>
    void Algorithm::StringBase<T>::Append(const StringBase& subString)
    {
        for (int i = 0; i < subString.Count(); i ++)
        {
            m_string.Add(subString[i]);
        }
        CStringlize();
    }

    template<typename T>
    Algorithm::StringBase<T> Algorithm::StringBase<T>::Substring(int start, int end)const
    {
        assert(end - start >= 0);
        StringBase<T> result;
        result.m_string.Replace(m_string.Begin() + start, end - start );
        result.CStringlize();
        return result;
    }

    template<typename T>
    Algorithm::Vector<Algorithm::StringBase<T>> Algorithm::StringBase<T>::Split(T delimiter) const
    {
        Algorithm::Vector<StringBase<T>> arr;
        if (m_string.Count() != 0)
        {
            int start = 0;
            int end = IndexOf(delimiter);
            while (end != INVALID_VALUE)
            {
                StringBase<T> token = Substring(start, end);
                if (token.Count() != 0) //-V728
                    arr.Add(token);
                start = end + 1;
                end = IndexOf(delimiter, start);
            }
            StringBase<T> token = Substring(start, Count());
            if (token.Count() != 0) //-V728
                arr.Add(token);
        }
        return arr;
    }

    template<typename T>
    void Algorithm::StringBase<T>::Resize(int length)
    {
        m_string.Resize(length);
        CStringlize();
    }

    template<typename T>
    void Algorithm::StringBase<T>::CStringlize()
    {
        if (m_string.IsCapacityEnoughToPutCStringEnd() == false) {
            m_string.DoubleCurrentCapacity();
        }
        m_string[m_string.Count()] = '\0';
    }

}