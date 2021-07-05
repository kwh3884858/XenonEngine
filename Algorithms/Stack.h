#ifndef SAINTMATHEMATICS_STACK_H
#define SAINTMATHEMATICS_STACK_H

namespace Algorithm {
	template<typename T>
	class Stack
	{
	private:
		static const int DEFAULTCapacity;
	public:
		Stack();
		~Stack();

		void Push(const T& value);
		T Pop();
		void Clear();

		int Count()const;
		int Capacity()const;

		T& operator[](int index);
		const T& operator[](int index)const;

	private:
		bool Initialize(int size);
		bool Reallocation();
		bool IsCapacityEnough()const;

		T* m_stack;
		int m_count;
		int m_capacity;
	};

	//-------------------define
	
	template<typename T>
	const int Stack<T>::DEFAULTCapacity = 1;
	
	template<typename T>
	inline Stack<T>::Stack()
	{

		m_count = 0;
		m_capacity = 0;
		m_stack = nullptr;

		Initialize(DEFAULTCapacity);
	}

	template<typename T>
	inline Stack<T>::~Stack()
	{
		Clear();
	}

	template<typename T>
	inline void Stack<T>::Push(const T & value)
	{
		if (IsCapacityEnough() == false)
		{
			Reallocation();
		}

		m_stack[m_count] = value;
		
		m_count++;

	}

	template<typename T>
	inline T Stack<T>::Pop()
	{
        assert(m_count > 0);

		T t = m_stack[m_count - 1];

		m_count--;

		return t;
	}

	template<typename T>
	inline void Stack<T>::Clear()
	{
		if (m_stack != nullptr)
		{
			//for (int i = 0; i < m_count; i++)
			//{
			//	m_stack[i] = nullptr;
			//}

			delete m_stack;
			m_stack = nullptr;
			m_count = 0;
		}
	}

	template<typename T>
	inline int Stack<T>::Count() const
	{
		return m_count;
	}

	template<typename T>
	inline int Stack<T>::Capacity() const
	{
		return m_capacity;
	}

	template<typename T>
	inline T& Stack<T>::operator[](int index)
	{
		return const_cast<T&>(static_cast<const Stack<T>&>(*this)[index]);
	}

	template<typename T>
	inline const T& Stack<T>::operator[](int index) const
	{
		assert(index >= 0 && index < m_count);
		return m_stack[index];
	}

	template<typename T>
	inline bool Stack<T>::Initialize(int size)
	{
		if (size <= 0)
		{
			return false;
		}

		m_capacity = size;

		m_count = 0;

		m_stack = new T[m_capacity];

		return true;
	}

	template<typename T>
	inline bool Stack<T>::Reallocation()
	{
		if (m_capacity == 0)
		{
			return false;
		}

		m_capacity = 2 * m_capacity;

		T* newContent = new T[m_capacity];

		for (int i = 0; i < m_count; i++)
		{
			newContent[i] = m_stack[i];
		}

		delete[] m_stack;

		m_stack = newContent;

		return true;
	}

	template<typename T>
	inline bool Stack<T>::IsCapacityEnough() const
	{
		return m_count < m_capacity;
	}

}

#endif