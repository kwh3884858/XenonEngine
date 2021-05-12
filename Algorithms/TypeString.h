#pragma once
#ifndef ENUMSTRING_H
#define ENUMSTRING_H

#include "Algorithms/Vector.h"
#include "Algorithms/String.h"
#include <cassert>
namespace Algorithm
{
    template <typename Type>
    struct TypeString
    {
        char const *    m_string;
        Type            m_typeVal;
    };

    template<typename Type>
    struct TypeStringStorageType
    {
        using Type = int;
    };

    template<typename Type, size_t ArraySize>
    struct TypeStringHelper
    {
        typename TypeStringStorageType<Type>::Type m_typeVal[ArraySize] = { };
        char const* m_string[ArraySize] = { };

        struct Iterator
        {
            TypeStringHelper const* m_container;
            int m_index;

            Iterator(TypeStringHelper const* container, int index)
                : m_container(container)
                , m_index(index)
            {}

            int operator ++()
            {
                m_index = m_index + 1;
                return m_index;
            }

            bool operator !=(Iterator const &other) const
            {
                assert(m_container == other.m_container);
                return m_index != other.m_index;
            }

            TypeString<Type> operator*() const
            {
                return m_container->operator[](m_index);
            }
        };

        Iterator begin() const
        {
            return Iterator(this, 0);
        }

        Iterator end() const
        {
            return Iterator(this, ArraySize);
        }

        TypeString<Type> operator[](int index) const
        {
            return TypeString<Type> { m_string[index], static_cast<Type>(m_typeVal[index]) };
        }

        size_t Size() const
        {
            return ArraySize;
        }
    };

    template<typename Type, size_t ArraySize>
    constexpr TypeStringHelper<Type, ArraySize> CreateSortedEnumStringArray(TypeString<Type> const (&enumStrArray)[ArraySize])
    {
        TypeStringHelper<Type, ArraySize> holder;

        for (int i = 0; i < ArraySize; ++i)
        {
            holder.m_typeVal[i] = enumStrArray[i].m_typeVal;
            holder.m_string[i] = enumStrArray[i].m_string;
        }

        for (int i = 0; i < ArraySize - 1; ++i)
        {
            for (int j = 0; j < ArraySize - i - 1; ++j)
            {
                if (holder.m_typeVal[j] > holder.m_typeVal[j + 1])
                {
                    auto enumVal = holder.m_typeVal[j];
                    holder.m_typeVal[j] = holder.m_typeVal[j + 1];
                    holder.m_typeVal[j + 1] = enumVal;

                    auto string = holder.m_string[j];
                    holder.m_string[j] = holder.m_string[j + 1];
                    holder.m_string[j + 1] = string;
                }
            }
        }

        return holder;
    }

    template <typename Type, size_t ArraySize>
    bool StringToType(TypeString<Type> const (&enumStrArray)[ArraySize], char const * str, Type & enumVal)
    {
        size_t u = 0;
        while (u < ArraySize && stricmp(enumStrArray[u].m_string, str) != 0)
        {
            ++u;
        }

        if (u < ArraySize)
        {
            enumVal = enumStrArray[u].m_typeVal;
            return true;
        }

        return false;
    }

    template <typename Type, size_t ArraySize>
    bool StringToType(TypeString<Type> const (&enumStrArray)[ArraySize], const String& str, Type & enumVal)
    {
        size_t u = 0;
        while (u < ArraySize && str == enumStrArray[u]) != 0)
        {
            ++u;
        }

        if (u < ArraySize)
        {
            enumVal = enumStrArray[u].m_typeVal;
            return true;
        }

        return false;
    }

    template <typename Type, size_t ArraySize>
    char const * EnumToString(TypeString<Type> const (&enumStrArray)[ArraySize], Type enumVal)
    {
        size_t u = 0;
        while (u < ArraySize && enumStrArray[u].m_typeVal != enumVal)
        {
            ++u;
        }

        if (u < ArraySize)
        {
            return enumStrArray[u].m_string;
        }

        return NULL;
    }

    template <typename Type, size_t ArraySize>
    char const * EnumToString(TypeStringHelper<Type, ArraySize> const& helper, Type enumVal)
    {
        auto end = helper.m_typeVal + ArraySize;
        auto it = std::lower_bound(helper.m_typeVal, end, static_cast<typename TypeStringStorageType<Type>::Type>(enumVal));

        if (it != end && *it == enumVal)
        {
            size_t index = it - helper.m_typeVal;
            return helper.m_string[index];
        }

        return nullptr;
    }

    template <typename Type, size_t ArraySize>
    char const * EnumToString(TypeStringHelper<Type, ArraySize> const& helper, Type enumVal, typename TypeStringStorageType<Type>::Type maxVal)
    {
        if (static_cast<typename TypeStringStorageType<Type>::Type>(enumVal) >= maxVal)
        {
            return nullptr;
        }

        return EnumToString(helper, enumVal);
    }
}
#endif // ENUMSTRING_H
