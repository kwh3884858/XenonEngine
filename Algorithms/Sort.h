#pragma once

namespace XenonAlgorithm
{

	template<typename T>
	class Sort
	{
	public:
		Sort();
		~Sort();

		void Selection(const T* begin, const T* end)const;
		void Insertion
	private:
		void Swap(const T* lhs, const T* rhs)const;
	};

	template<typename T>
	void XenonAlgorithm::Sort<T>::Swap(const T* lhs, const T* rhs) const
	{
		T tmp = *lhs;
		*lhs = *rhs;
		*rhs = tmp;
	}

	template<typename T>
	void XenonAlgorithm::Sort<T>::Selection(const T* begin, const T* end) const
	{
		for ( ; begin != end; begin++ )
		{
			const T* smallest = begin;

			for (const T* iter = begin; iter != end; iter++)
			{
				if ( *smallest > *iter)
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