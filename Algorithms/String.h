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

    template<typename T>
    bool operator==(const StringBase<T>& lhs, const StringBase<T>& rhs);
	template<typename T>
	bool Swap(StringBase<T>& lhs, StringBase<T>& rhs);

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

        void Add(T value);
        int Count()const;
        int ToInt()const;
        float ToFloat()const;
        char ToChar()const;
        void CString(char*const pOutChar) const;
        void Clear();
        const T* const Beign()const;

        int IndexOf(const StringBase& subString)const;
        bool Find(const StringBase& subString)const;
        void Append(const StringBase& subString);
        StringBase<T> Substring(unsigned int start, unsigned int end);
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
            index++;
        }
    }

    template<typename T>
    inline StringBase<T>::StringBase(T value)
    {
        m_string.Add(value);
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
    bool Algorithm::StringBase<T>::operator==(const StringBase<T>& rhs)const
    {
        if (Count() != rhs.Count())
        {
            return false;
        }
        int minCount = Count() < rhs.Count() ? Count() : rhs.Count();
        for (int i = 0; i < minCount; i++)
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
        float result = atof(content);
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
    inline void StringBase<T>::CString(char*const pOutChar) const
    {
        memcpy(pOutChar, m_string.Begin(), m_string.Count());
        pOutChar[m_string.Count()] = '\0';
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
                pOutDFA->m_character[nonCharacterCount++] = i;
            }
        }

        const int startPos = 0;
        const int nextPosWhenFirstCharacterIsCorrect = 1;
        pOutDFA->Set(pOutDFA->GetCharacterPos(subString[0]), startPos, nextPosWhenFirstCharacterIsCorrect);
        
        int restartPos = 0;
        for (int currentSubStringIndex = 1; currentSubStringIndex < pOutDFA->m_countentlength; currentSubStringIndex++) {
            for (int character = 0; character < pOutDFA->m_characterCount; character++) {
                pOutDFA->Set(character, currentSubStringIndex, pOutDFA->Get(character, restartPos));
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
        return -1;
    }

    template<typename T>
    inline bool StringBase<T>::Find(const StringBase& subString)const
    {
        return IndexOf(subString) != -1;
    }

    template<typename T>
    void Algorithm::StringBase<T>::Append(const StringBase& subString)
    {
        for (int i = 0; i < subString.Count(); i ++)
        {
            m_string.Add(subString[i]);
        }
    }

    template<typename T>
    Algorithm::StringBase<T> Algorithm::StringBase<T>::Substring(unsigned int start, unsigned int end)
    {
        StringBase<T> result;
        result.m_string.Replace(m_string.Begin() + start, end - start );
        return result;
    }

	typedef StringBase<char> String;
}