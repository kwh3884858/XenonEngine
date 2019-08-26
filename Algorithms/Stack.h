#ifndef SAINTMATHEMATICS_STACK_H
#define SAINTMATHEMATICS_STACK_H

namespace XenonAlgorithm {
	template<typename T>
	class Stack
	{
	private:
		static const int DEFAULTCAPACITY;
	public:
		Stack();
		~Stack();

		void push(const T& value);
		T pop();
		void clear();

		int count()const;
		int capacity()const;

	private:
		bool initialize(int size);
		bool reallocation();
		bool isCapacityEnough()const;

		T* mStack;
		int mCount;
		int mCapacity;
	};

	//-------------------define
	
	template<typename T>
	const int Stack<T>::DEFAULTCAPACITY = 1;
	
	template<typename T>
	inline Stack<T>::Stack()
	{

		mCount = 0;
		mCapacity = 0;
		mStack = nullptr;

		initialize(DEFAULTCAPACITY);
	}

	template<typename T>
	inline Stack<T>::~Stack()
	{
		clear();
	}

	template<typename T>
	inline void Stack<T>::push(const T & value)
	{
		if (isCapacityEnough() == false)
		{
			reallocation();
		}

		mStack[mCount] = value;
		
		mCount++;

	}

	template<typename T>
	inline T Stack<T>::pop()
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
	inline void Stack<T>::clear()
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
	inline int Stack<T>::count() const
	{
		return mCount;
	}

	template<typename T>
	inline int Stack<T>::capacity() const
	{
		return mCapacity;
	}

	template<typename T>
	inline bool Stack<T>::initialize(int size)
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
	inline bool Stack<T>::reallocation()
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
	inline bool Stack<T>::isCapacityEnough() const
	{
		return mCount < mCapacity;
	}

}

#endif