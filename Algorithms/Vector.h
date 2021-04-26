#ifndef SAINTMATHEMATICS_VECOTR_H
#define SAINTMATHEMATICS_VECOTR_H

//class Iterator;
#include <cstring>

namespace Algorithm
{
	template<typename T>
	class Vector
	{
	private:
		static const int DEFAULTCAPACITY;

	public:
		Vector();
		Vector(const Vector& that);
		~Vector();

		bool Add(const T& element);
		bool Remove(const T& element);
        bool Replace(const T*const content,unsigned int size);
		bool Clear();
		int IndexOf(T element)const;
		int Count()const;
		int Capacity()const;

		T operator[](int index);

		//For Algorithm
		bool Swap(const int lhs, const int rhs);
		T* begin();
        const T* begin()const;
        T* end();
        const T* end()const;

	private:
		bool Initialize(int size);
		bool IsCapacityEnough();
		bool Reallocation();
        bool InternalReplace(const T*const content, unsigned int size);

		T* m_content;
		int m_count;
		int m_capacity;

	};

	template<typename T>
	const int Vector<T>::DEFAULTCAPACITY = 1;

	template<typename T>
	Vector<T>::Vector()
	{
		m_content = nullptr;
		m_count = 0;
		m_capacity = 0;
		Initialize(DEFAULTCAPACITY);
	}

    template<typename T>
    Vector<T>::Vector(const Vector& that)
    {
        Initialize(that.Capacity());
        InternalReplace(that.m_content, that.m_count);
    }

	template<typename T>
	Vector<T>::~Vector()
	{
		Clear();
	}

	template<typename T>
	bool Vector<T>::Initialize(int size)
	{
		if (size <= 0)
		{
			return false;
		}
		m_capacity = size;
		m_count = 0;
		m_content = new T[m_capacity];
		return true;
	}

	template<typename T>
	bool Vector<T>::Clear()
	{
		if (m_content)
		{
			for (int i = 0; i < m_count; i++)
			{
				m_content[i] = 0;
			}
			delete[] m_content;
			m_content = nullptr;
			m_count = 0;
		}
		return true;
	}

	template<typename T>
	bool Vector<T>::Add(const T & element)
	{
		if (IsCapacityEnough() == false) {
			Reallocation();

		}
		m_content[m_count] = element;
		m_count++;
		return true;
	}

	template<typename T>
	bool Vector<T>::Remove(const T & element)
	{
		for (int i = m_count - 1; i >= 0; i--)
		{
			if (m_content[i] == element) 
            {
				m_content[i] = nullptr;
				for (int j = i; j < m_count - 1; j++)
				{
					m_content[j] = m_content[j + 1];
				}
				m_content[m_count - 1] = nullptr;
				m_count--;
			}
		}
		return true;
	}

    template<typename T>
    bool Algorithm::Vector<T>::Replace(const T*const content, unsigned int size)
    {
        if (m_capacity <= size)
        {
            Clear();
            Initialize(size);
        }
        InternalReplace(content, size);
    }

	template<typename T>
	int Vector<T>::IndexOf(T element)const
	{
		if (m_count == 0) return -1;
		for (int i = m_count - 1; i >= 0; i++)
		{
			if (m_content[i] == element) {
				return i;
			}
		}
		return -1;
	}

	template<typename T>
	inline int Vector<T>::Count()const
	{
		return m_count;
	}

	template<typename T>
	inline int Vector<T>::Capacity()const
	{
		return m_capacity;
	}

	template<typename T>
	inline T Vector<T>::operator[](int index)
	{
		if (index <0 || index >= m_count)
		{
			return nullptr;
		}
		return m_content[index];
	}
    
    template<typename T>
    const T* Vector<T>::begin()const
    {
        return m_content;
    }

    template<typename T>
    T*  Vector<T>::begin()
    {
        return const_cast<T*>(static_cast<const Vector<T>&>(*this).begin());
    }
    
    template<typename T>
    const T* Vector<T>::end()const
    {
        if (m_count == 0)
        {
            return nullptr;
        }
        return &(m_content[m_count - 1]);
    }

    template<typename T>
    T* Vector<T>::end()
    {
        return const_cast<T*>(static_cast<const Vector<T>&>(*this).end());
    }

	template<typename T>
	bool Vector<T>::IsCapacityEnough()
	{
		return m_count < m_capacity;
	}

	template<typename T>
	bool Vector<T>::Reallocation()
	{
		if (m_capacity == 0)
		{
			return false;
		}
		m_capacity = 2 * m_capacity;
		T* newContent = new T[m_capacity];
		for (int i = 0; i < m_count; i++)
		{
			newContent[i] = m_content[i];
		}
		delete[] m_content;
		m_content = newContent;
		return true;
	}

    template<typename T>
    bool Algorithm::Vector<T>::InternalReplace(const T*const content, unsigned int size)
    {
        memcpy(m_content, content, size);
        m_count = size;
    }

}


#endif