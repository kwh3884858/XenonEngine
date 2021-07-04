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

		T* mStack;
		int mCount;
		int mCapacity;
	};

	//-------------------define
	
	template<typename T>
	const int Stack<T>::DEFAULTCapacity = 1;
	
	template<typename T>
	inline Stack<T>::Stack()
	{

		mCount = 0;
		mCapacity = 0;
		mStack = nullptr;

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

		mStack[mCount] = value;
		
		mCount++;

	}

	template<typename T>
	inline T Stack<T>::Pop()
	{
		if (mCount <= 0)
		{
			return nullptr;
		}

		T t = mStack[mCount - 1];

		mCount--;

		return t;
	}

	template<typename T>
	inline void Stack<T>::Clear()
	{
		if (mStack != nullptr)
		{
			for (int i = 0; i < mCount; i++)
			{
				mStack[i] = nullptr;
			}

			delete mStack;
			mStack = nullptr;
			mCount = 0;
		}
	}

	template<typename T>
	inline int Stack<T>::Count() const
	{
		return mCount;
	}

	template<typename T>
	inline int Stack<T>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	inline T& Stack<T>::operator[](int index)
	{
		return const_cast<T&>(const_cast<const Stack<T>*>(this)[index]);
	}

	template<typename T>
	inline const T& Stack<T>::operator[](int index) const
	{
		assert(index >= 0 && index < m_count);
		return m_content[index];
	}

	template<typename T>
	inline bool Stack<T>::Initialize(int size)
	{
		if (size <= 0)
		{
			return false;
		}

		mCapacity = size;

		mCount = 0;

		mStack = new T[mCapacity];

		return true;
	}

	template<typename T>
	inline bool Stack<T>::Reallocation()
	{
		if (mCapacity == 0)
		{
			return false;
		}

		mCapacity = 2 * mCapacity;

		T* newContent = new T[mCapacity];

		for (int i = 0; i < mCount; i++)
		{
			newContent[i] = mStack[i];
		}

		delete[] mStack;

		mStack = newContent;

		return true;
	}

	template<typename T>
	inline bool Stack<T>::IsCapacityEnough() const
	{
		return mCount < mCapacity;
	}

}

#endif