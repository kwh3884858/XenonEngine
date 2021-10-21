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
        
        typedef bool CampareFunction(const T& lhs, const T& rhs);

		void Selection(const T* begin, const T* end) const;
		void Insertion(const T* begin, const T* end) const;
		void Shell(const T* begin, const T* end) const;
		void UpDownSortion(const T* begin, const T* end) const;
		void DownUpSortion(const T* begin, const T* end) const;
		void Quick(const T* begin, const T* end, CampareFunction* isLess) const;
		void Quick( T*const begin, int length, CampareFunction* isLess) const;
	private:
		bool Less(const T* lhs, const T* rhs)const;
		void Swap( T* const lhs,  T*const rhs)const;
		void Swap( T& lhs,  T& rhs)const;
		void Merge(const T* begin, int low, int middle, int high) const;
		int Length(const T* begin, const T* end) const;
		void DownSortion(const T* begin, int low ,int high) const;

        void InternalQuick( T*const begin, int low, int high, CampareFunction* isLess) const;
        int Partition( T*const begin, int low, int high, CampareFunction* isLess) const;

	};

	template<typename T>
	bool Algorithm::Sort<T>::Less(const T* lhs, const T* rhs) const
	{
		return *lhs < *rhs;
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
            begin++;
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
    void Algorithm::Sort<T>::Quick(const T* begin, const T* end, CampareFunction* isLess) const
    {
        int length = Length(begin, end);
        InternalQuick(begin, 0, length - 1, isLess);
    }

    template<typename T>
    void Algorithm::Sort<T>::Quick( T*const begin, int length, CampareFunction* isLess) const
    {
        InternalQuick(begin, 0, length - 1, isLess);
    }

    template<typename T>
    void Algorithm::Sort<T>::InternalQuick( T*const begin, int low, int high, CampareFunction* isLess) const
    {
        if (low >= high)
        {
            return;
        }
        int middle = Partition(begin, low, high, isLess);
        InternalQuick(begin, low, middle - 1, isLess);
        InternalQuick(begin, middle + 1, high, isLess);
    }

    template<typename T>
    int Algorithm::Sort<T>::Partition( T*const begin, int low, int high, CampareFunction* isLess)const
    {
        //from low + 1 to high
        int left = low;
        int right = high + 1;
         T middle = begin[low];
        while (true)
        {
            while (isLess(begin[++left], middle))
            {
                if (left == high) break;
            }
            while (isLess(middle, begin[--right]))
            {
                if (right == low) break;
            }

            if (left >= right)
            {
                Swap(begin[low], begin[right]);
                break;
            }
            Swap(begin[left], begin[right]);
        }
        return right;
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
	void Algorithm::Sort<T>::Swap( T* const lhs,  T*const rhs) const
	{
		T tmp = *lhs;
		*lhs = *rhs;
		*rhs = tmp;
	}

    template<typename T>
    void Algorithm::Sort<T>::Swap( T& lhs,  T& rhs) const
    {
        Swap(&lhs, &rhs);
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