#ifndef SAINTMATHEMATICS_VECOTR_H
#define SAINTMATHEMATICS_VECOTR_H

//class Iterator;

namespace Algorithm
{
	template<typename T>
	class Vector
	{
	private:
		static const int DEFAULTCAPACITY;

	public:
		Vector();
		Vector(const Vector&) = delete;
		~Vector();

		bool Add(const T& element);
		bool Remove(const T& element);
		bool Clear();
		int IndexOf(T element);
		int Count();
		int Capacity();

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

		T* mContent;

		int mCount;

		int mCapacity;

	};


    template<typename T>
    const T* Vector<T>::end()const
    {
        if (mCount == 0)
        {
            return nullptr;
        }
        return mContent[mCount - 1];
    }

    template<typename T>
    T* Vector<T>::end()
    {
        return const_cast<T*>(static_cast<const Vector<T>&>(*this).end());
    }

    template<typename T>
    const T* Vector<T>::begin()const
    {
        return mContent[0];
    }

    template<typename T>
    T*  Vector<T>::begin()
    {
        return const_cast<T*>(static_cast<const Vector<T>&>(*this).begin());
    }

	//template<typename T>
	//bool SaintMathematics::Vector<T>::Swap(const int lhs, const int rhs)
	//{
	//	if (lhs == rhs)
	//	{
	//		return true;
	//	}

	//	if (lhs <0 || lhs > mCount || rhs < 0 || rhs > mCount)
	//	{
	//		return false;
	//	}

	//	T tmp = mContent[lhs];
	//	mContent[lhs] = mContent[rhs];
	//	mContent[rhs] = tmp;
	//	return true;
	//}

	//-------------------define
	template<typename T>
	const int Vector<T>::DEFAULTCAPACITY = 1;

	template<typename T>
	Vector<T>::Vector()
	{
		mContent = nullptr;

		mCount = 0;

		mCapacity = 0;

		Initialize(DEFAULTCAPACITY);
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

		mCapacity = size;

		mCount = 0;

		mContent = new T[mCapacity];

		return true;
	}


	template<typename T>
	bool Vector<T>::Clear()
	{
		if (mContent)
		{
			for (int i = 0; i < mCount; i++)
			{
				mContent[i] = nullptr;
			}
			delete[] mContent;

			mContent = nullptr;

			mCount = 0;
		}

		return true;

	}





	template<typename T>
	bool Vector<T>::Add(const T & element)
	{


		if (IsCapacityEnough() == false) {
			Reallocation();

		}

		mContent[mCount] = element;

		mCount++;

		return true;


	}

	template<typename T>
	bool Vector<T>::Remove(const T & element)
	{

		for (int i = mCount - 1; i >= 0; i--)
		{

			if (mContent[i] == element) {
				mContent[i] = nullptr;

				for (int j = i; j < mCount - 1; j++)
				{
					mContent[j] = mContent[j + 1];
				}

				mContent[mCount - 1] = nullptr;

				mCount--;
			}

		}

		return true;

	}


	template<typename T>
	int Vector<T>::IndexOf(T element)
	{
		if (mCount == 0) return -1;

		for (int i = mCount - 1; i >= 0; i++)
		{

			if (mContent[i] == element) {
				return i;
			}

		}

		return -1;
	}

	template<typename T>
	inline int Vector<T>::Count()
	{
		return mCount;
	}

	template<typename T>
	inline int Vector<T>::Capacity()
	{
		return mCapacity;
	}

	template<typename T>
	inline T Vector<T>::operator[](int index)
	{
		if (index <0 || index >= mCount)
		{
			return nullptr;
		}
		
		return mContent[index];
	}




	template<typename T>
	bool Vector<T>::IsCapacityEnough()
	{
		return mCount < mCapacity;
	}

	template<typename T>
	bool Vector<T>::Reallocation()
	{
		if (mCapacity == 0)
		{
			return false;
		}

		mCapacity = 2 * mCapacity;

		T* newContent = new T[mCapacity];

		for (int i = 0; i < mCount; i++)
		{
			newContent[i] = mContent[i];
		}

		delete[] mContent;

		mContent = newContent;

		return true;
	}


}


#endif