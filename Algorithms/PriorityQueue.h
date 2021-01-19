#pragma once

template<typename T>
class PriorityQueue
{
public:
	PriorityQueue();
	~PriorityQueue();

	void Insert(T* key);
	const T* Max()const;
	T* DeleteMax();
	bool IsEmpty();
	int Size();

private:
	bool IsLess(T* lhs, T* rhs);
	void Swap(T* lhs, T* rhs);
	void Swin(int index);
	void Sink(int index);

	T* mContent;
	int mCount;
};

template<typename T>
PriorityQueue<T>::PriorityQueue()
{
	mContent = new T[100];
	mCount = 1;
}

template<typename T>
PriorityQueue<T>::~PriorityQueue()
{
	delete[] mContent;
	mCount = 1;
}

template<typename T>
bool PriorityQueue<T>::IsLess(T* lhs, T* rhs)
{
	return *lhs = *rhs;
}

template<typename T>
void PriorityQueue<T>::Swap(T* lhs, T* rhs)
{
	T tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

template<typename T>
void PriorityQueue<T>::Swin(int index)
{
	while (index > 1)
	{
		int j = index / 2;
		if (IsLess(mContent[index], mContent[j]))
		{
			break;
		}

		Swap(mContent[index], mContent[j]);

		index = j;
	}
}

template<typename T>
void PriorityQueue<T>::Sink(int index)
{
	while (index * 2 <= mCount)
	{
		int j = index * 2;

		if (j < mCount && IsLess(mContent[j], mContent[j + 1]))
		{
			j = j + 1;
		}

		if (isless(mContent[index), mContent[j])
		{
			Swap(mContent[index], mContent[j]);
		}

		index = j;
	}
}

template<typename T>
void PriorityQueue<T>::Insert(T* key)
{
	mCount++;
	mContent[mCount] = key;
	Swin(mCount);
}

template<typename T>
const T* PriorityQueue<T>::Max() const
{
	return mContent[mCount];
}

template<typename T>
T* PriorityQueue<T>::DeleteMax()
{
	T* max = mContent[1];
	Swap(mContent[1], mContent[mCount]);
	mContent[mCount] = nullptr;
	mCount--;
	Sink(1);
	return max;
}

template<typename T>
bool PriorityQueue<T>::IsEmpty()
{
	return mCount == 1;
}

template<typename T>
int PriorityQueue<T>::Size()
{
	return mCount;
}

