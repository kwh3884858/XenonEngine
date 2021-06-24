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
        StringBase<T>& operator=(const StringBase<T>& rhs);
        ~StreamingVector();

        void Add(const void*const pdata, int size, int count = 1);
    private:
        void Reallocation(int neededSpace);
    };

    template<typename T>
    void Algorithm::StreamingVector<T>::Add(const void*const pdata, int size, int count)
    {
        int neededSpace = size * count;
        int currentIndex = m_count - 1;
        m_count += neededSpace;
        if (!IsCapacityEnough())
        {
            Reallocation(neededSpace);
        }
        assert(m_count <= m_capacity);
        memcpy(m_content[currentIndex], pdata, neededSpace);

    }

    template<typename T>
    void Algorithm::StreamingVector<T>::Reallocation(int neededSpace)
    {
        assert(m_capacity != 0);

        int expandedSpace = m_capacity > neededSpace ? m_capacity : neededSpace;
        m_capacity = 2 * m_capacity;
        T* newContent = new T[m_capacity];
        for (int i = 0; i < m_count; i++)
        {
            newContent[i] = m_content[i];
        }
        delete[] m_content;
        m_content = newContent;
    }

}