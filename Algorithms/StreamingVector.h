#pragma once
#include "Algorithms/Vector.h"

namespace Algorithm
{
    template<typename T>
    class StreamingVector : public Vector<T>
    {
    public:
        StreamingVector();
        StreamingVector(const StreamingVector& value);
        ~StreamingVector();

        void Add(const void*const pdata, int size, int count = 1);
    private:
        void Reallocation(int neededSpace);
        bool IsCapacityEnough(int need)const;
    };

    template<typename T>
    inline StreamingVector<T>::StreamingVector():
        Vector<T>()
    {
    }

    template<typename T>
    inline StreamingVector<T>::StreamingVector(const StreamingVector& value):
        Vector<T>(value)
    {
    }

    template<typename T>
    inline StreamingVector<T>::~StreamingVector()
    {
    }

    template<typename T>
    void Algorithm::StreamingVector<T>::Add(const void*const pdata, int size, int count)
    {
        int neededSpace = size * count;
        if (!IsCapacityEnough(neededSpace))
        {
            Reallocation(neededSpace);
        }
        assert(m_count <= m_capacity);

        memcpy(m_content + m_count, pdata, neededSpace);
        m_count += neededSpace;
    }

    template<typename T>
    void Algorithm::StreamingVector<T>::Reallocation(int neededSpace)
    {
        assert(m_capacity != 0);

        int expandedSpace = m_capacity > neededSpace ? m_capacity : neededSpace;
        m_capacity = 2 * expandedSpace;
        T* newContent = new T[m_capacity];
        for (int i = 0; i < m_count; i++)
        {
            newContent[i] = m_content[i];
        }
        assert(newContent != nullptr);
        delete[] m_content;
        m_content = newContent;
    }

    template<typename T>
    inline bool StreamingVector<T>::IsCapacityEnough(int need) const
    {
        return m_count + need < m_capacity;
    }

}