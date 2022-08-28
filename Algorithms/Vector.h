#ifndef SAINTMATHEMATICS_VECOTR_H
#define SAINTMATHEMATICS_VECOTR_H

//class Iterator;
#include <cstring>
#include <cassert>

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
		Vector(Vector&& that);
		~Vector();

		bool Add(const T& element);
		bool Add(T&& element);
		bool Remove(const T& element);
		void Remove(int index);
		bool Initialize(int size);
        void Replace(const T*const content,int size);
        bool Clear();
        void Resize(int size);

		int IndexOf(T element)const;
		int Count()const;
		int Capacity()const;

        bool IsExist(const T& element)const;

		T& operator[](int index);
		const T& operator[](int index)const;
		Vector<T>& operator=(const Vector& rhs);
		Vector<T>& operator=(Vector&& rhs);

		//For Algorithm
		T* Begin();
        const T* Begin()const;
        T* End();
        const T* End()const;
		// For std iteration
		T* begin() { return Begin(); }
		const T* begin()const { return Begin(); }
		T* end() { return End(); }
		const T* end()const { return End(); }

        bool IsCapacityEnoughToPutCStringEnd()const;
        void DoubleCurrentCapacity();
	protected:
		bool IsCapacityEnough() const;
		T* m_content = nullptr;
		int m_count;
		int m_capacity;

	private:
		bool Reallocation();
        bool InternalReplace(const T*const content, int size);
        bool Destory();
	};

	template<typename T>
	const int Vector<T>::DEFAULTCAPACITY = 3;

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
	Algorithm::Vector<T>::Vector(Vector&& that)
	{
		Initialize(that.Capacity());
		InternalReplace(that.m_content, that.m_count);

		that.m_content = nullptr;
		that.m_count = 0;
		that.m_capacity = 0;
	}

	template<typename T>
	Vector<T>::~Vector()
	{
		Destory();
	}

	template<typename T>
	bool Vector<T>::Initialize(int size)
	{
		assert(size > 0);

		m_capacity = size;
		m_count = 0;
		m_content = new T[m_capacity];
		return true;
	}

    template<typename T>
    bool Algorithm::Vector<T>::Clear()
    {
        //for (int i = 0; i < m_count; i++)
        //{
        //    m_content[i];
        //}
        m_count = 0;
        return true;
    }

    template<typename T>
    void Algorithm::Vector<T>::Resize(int size)
    {
        if (size < m_count)
        {
            m_count = size;
        }
        if (size > m_count)
        {
            for (int i = m_count; i < size; i++)
            {
                Add(T());
            }
        }
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
	bool Algorithm::Vector<T>::Add(T&& element)
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
		bool result = false;
		for (int i = m_count - 1; i >= 0; i--)
		{
			if (m_content[i] == element) 
            {
				Remove(i);
				result = true;
			}
		}
		return result;
	}

	template<typename T>
	inline void Vector<T>::Remove(int index)
	{
		assert(index >= 0 && index < m_count);
		m_content[index] = nullptr;
		for (int j = index; j < m_count - 1; j++)
		{
			m_content[j] = m_content[j + 1];
		}
		m_content[m_count - 1] = nullptr;
		m_count--;
	}

    template<typename T>
    void Algorithm::Vector<T>::Replace(const T*const content, int size)
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
		for (int i = m_count - 1; i >= 0; i--)
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
    bool Algorithm::Vector<T>::IsExist(const T& element) const
    {
        for (int i = 0; i< Count(); i++ )
        {
            if (m_content[i] == element)
            {
                return true;
            }
        }
        return false;
    }

	template<typename T>
	inline T& Vector<T>::operator[](int index)
	{
        return const_cast<T&>(static_cast<const Vector<T>&>(*this)[index]);
	}

	template<typename T>
	inline const T& Vector<T>::operator[](int index) const
	{
        assert(index >= 0 && index < m_capacity);
		return m_content[index];
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}
		Destory();
		Initialize(rhs.Capacity());
		InternalReplace(rhs.m_content, rhs.m_count);
		return *this;
	}

	template<typename T>
	Algorithm::Vector<T>& Algorithm::Vector<T>::operator=(Vector&& rhs)
	{
		if (this == &rhs)
			return *this;

		Destory();
		Initialize(rhs.Capacity());
		InternalReplace(rhs.m_content, rhs.m_count);

		rhs.m_content = nullptr;
		rhs.m_count = 0;
		rhs.m_capacity = 0;

		return *this;
	}

    template<typename T>
    const T* Vector<T>::Begin()const
    {
        return m_content;
    }

    template<typename T>
    T*  Vector<T>::Begin()
    {
        return const_cast<T*>(static_cast<const Vector<T>&>(*this).Begin());
    }
    
    template<typename T>
    const T* Vector<T>::End()const
    {
        if (m_count == 0)
        {
            return nullptr;
        }
        return &(m_content[m_count - 1]);
    }

    template<typename T>
    T* Vector<T>::End()
    {
        return const_cast<T*>(static_cast<const Vector<T>&>(*this).End());
    }

    template<typename T>
    bool Algorithm::Vector<T>::IsCapacityEnoughToPutCStringEnd() const
    {
        return m_count + 1 < m_capacity;
    }

    template<typename T>
    void Algorithm::Vector<T>::DoubleCurrentCapacity()
    {
        Reallocation();
    }

	template<typename T>
	bool Vector<T>::IsCapacityEnough()const
	{
		return m_count < m_capacity;
	}

	template<typename T>
	bool Vector<T>::Reallocation()
	{
        assert(m_capacity > 0);
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
    bool Algorithm::Vector<T>::InternalReplace(const T*const content, int size)
    {
        assert(m_content != nullptr);
        assert(content != nullptr);
        for (int i = 0; i < size; i++)
        {
            m_content[i] = content[i];
        }
        //memcpy(m_content, content, size * sizeof(T));
        m_count = size;
        return true;
    }

    template<typename T>
    bool Vector<T>::Destory()
    {
        if (m_content)
        {
            delete[] m_content;
            Clear();
            m_content = nullptr;
        }
        return true;
    }
}


#endif