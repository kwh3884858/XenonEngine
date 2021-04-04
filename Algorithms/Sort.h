#pragma once

#include <vector>


namespace Algorithm
{

	template<typename T>
	class Sort
	{
	public:
		Sort();
		~Sort();

		void Selection(const T* begin, const T* end) const;
		void Insertion(const T* begin, const T* end) const;
		void Shell(const T* begin, const T* end) const;
		void UpDownSortion(const T* begin, const T* end) const;
		void DownUpSortion(const T* begin, const T* end) const;
		void Quick() const;
	private:
		bool Less(const T* lhs, const T* rhs)const;
		void Swap(const T* lhs, const T* rhs)const;
		void Merge(const T* begin, int low, int middle, int high) const;
		int Length(const T* begin, const T* end) const;
		void DownSortion(const T* begin, int low ,int high) const;
		void Partition(const T* begin, int low, int high) const;
	};

	template<typename T>
	bool Algorithm::Sort<T>::Less(const T* lhs, const T* rhs) const
	{
		return *lhs < *rhs;
	}

	template<typename T>
	void Algorithm::Sort<T>::Partition(const T* begin, int low, int high)const
	{
		int i = low;
		int j = high + 1;
		T* middle = begin[0];
		while (true)
		{
			while (Less(begin[++i], middle))
			{
				if (i == high)
				{
					break;
				}
			}

			while (Less(middle ,begin[--j]))
			{
				if ()
				{
				}
			}
		}
	}

	template<typename T>
	void Algorithm::Sort<T>::Quick()const
	{

	}

	template<typename T>
	void Algorithm::Sort<T>::DownSortion(const T* begin, int low, int high)const
	{
		if (low >= high)
		{
			return;
		}

		int middle = low + ( high - low) / 2;
		DownSortion(low, middle);
		DownSortion(middle + 1, high);

		Merge(begin, low, middle, high);
	}

	template<typename T>
	int Algorithm::Sort<T>::Length(const T* begin, const T* end)const
	{
		int length = 0;
		while (begin != end)
		{
			length++;
		}
		return length;
	}

	template<typename T>
	void Algorithm::Sort<T>::DownUpSortion(const T* begin, const T* end)const
	{
		int length = Length(begin, end);
		for (int i = 1; i < length; i += i)
		{
			for (int j = 0; j < length - i; j += i+i)
			{
				if (j + i+i < length)
				{
					Merge(begin, j, j + i - 1, j + i + i - 1);
				}
				else
				{
					Merge(begin, j, j + i - 1, length - 1);
				}
			
			}
		}
	}

	template<typename T>
	void Algorithm::Sort<T>::UpDownSortion(const T* begin, const T* end)const
	{
		int length = Length(begin, end);

		DownSortion(begin, 0, length - 1)
	}

	template<typename T>
	void Algorithm::Sort<T>::Merge(const T* begin, int low, int middle, int high)const
	{
		vector<T*> tmp;
		int i = low;
		int j = middle + 1;

		for (int k = low; k <= high ; k++)
		{
			tmp.push_back(begin[k]);
		}

		for (int k = low; k <= high; k++)
		{
			if ( i > middle)
			{
				begin[k] = tmp[j];
				j++;
			}
			else if ( j > high)
			{
				begin[k] = tmp[i];
				i++;
			}
			else if ( Less( tmp[i] ,tmp[j] ) )
			{
				begin[k] = tmp[i];
				i++;
			}
			else
			{
				begin[k] = tmp[j];
				j++
			}
		}

	}

	template<typename T>
	void Algorithm::Sort<T>::Shell(const T* begin, const T* end) const
	{
		int length = 0;
		int height = 0;

		length = Length(begin, end);

		while (height < length / 3)
		{
			height = height * 3 + 1;
		}

		while (height >= 1)
		{
			for (int i = height; i < length ; i++)
			{
				for (int j = i; j >= height ; j-= height)
				{
					if ( less(begin[j] , begin[j - height]) )
					{
						Swap(j, j - height);
					}
					else
					{
						break;
					}
				}
			}

			height = height / 3;
		}
	}

	template<typename T>
	void Algorithm::Sort<T>::Insertion(const T* begin, const T* end) const
	{
		const T* iter = begin;
		for (int i = 0; iter != end; iter++, i++)
		{
			
			for (int j = i; j > 0 ; j--)
			{
				if ( Less(begin + j - 1, begin + j) )
				{
					Swap(begin + j - 1, begin + j);
				}
				else
				{
					break;
				}
			}
		}
	}

	template<typename T>
	void Algorithm::Sort<T>::Swap(const T* lhs, const T* rhs) const
	{
		T tmp = *lhs;
		*lhs = *rhs;
		*rhs = tmp;
	}

	template<typename T>
	void Algorithm::Sort<T>::Selection(const T* begin, const T* end) const
	{
		for (; begin != end; begin++)
		{
			const T* smallest = begin;

			for (const T* iter = begin; iter != end; iter++)
			{
				if (*smallest > *iter)
				{
					smallest = iter;
				}
			}

			if (smallest == begin)
			{
				continue;
			}

			Swap(smallest, begin);
		}
	}

	template<typename T>
	Sort<T>::Sort<T>()
	{
	}

	template<typename T>
	Sort<T>::~Sort<T>()
	{
	}

}