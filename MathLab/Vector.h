#pragma once

namespace MathLab
{
	template<typename T, int COUNT>
	struct TVector;

	template<typename T, int COUNT>
	TVector<T, COUNT> operator+(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs);
	template<typename T, int COUNT>
	TVector<T, COUNT> operator-(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs);
	template<typename T, int COUNT>
	TVector<T, COUNT> operator*(const T& lhs, const TVector<T, COUNT>& value);

	template<typename T, int COUNT>
	void SwapVector(TVector<T, COUNT>& vectorA, TVector<T, COUNT>& vectorB);

	template<typename T, int COUNT>
	class TVector
	{
	public:
		T* m_vector;

	public:
		static TVector Zero;

		friend void SwapVector<T, COUNT>(TVector<T, COUNT>& vectorA, TVector<T, COUNT>& vectorB);

		friend TVector<T, COUNT> operator+ <>(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs);
		friend TVector<T, COUNT> operator- <>(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs);
		
		T& operator[](int index);
		const T& operator[](int index)const;
		TVector operator-()const;
		TVector& operator=(const TVector& that);
		TVector& operator+=(const TVector& that);
		TVector& operator-=(const TVector& that);
		TVector& operator*=(T value);

		TVector operator*(T value)const;
		TVector operator/(T value)const;

		bool operator==(const TVector& that);

		T Dot(const TVector& that)const;
		//T Cross(const Vector& that)const;

		TVector();
		TVector(const TVector& that);
		~TVector();
		
		inline int Count()const { return COUNT; }
		TVector Normalize()const;
		T Magnitude()const;
		T DoubleMagnitude()const;
	};

	template<typename T, int COUNT>
	__declspec(selectany) TVector<T, COUNT> TVector<T, COUNT>::Zero = TVector<T, COUNT>();


	template<typename T, int COUNT>
	TVector<T, COUNT>::TVector()
	{
		m_vector = new T[COUNT];
		memset(m_vector, 0, COUNT * sizeof(T));
	}

	template<typename T, int COUNT>
	TVector<T, COUNT>::TVector(const TVector& that)
	{
		TVector();
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] = that[i];
		}
	}

	template<typename T, int COUNT>
	TVector<T, COUNT>::~TVector()
	{
		delete[] m_vector;
		m_vector = nullptr;
	}

	template<typename T, int COUNT>
	T& MathLab::TVector<T, COUNT>::operator[](int index)
	{
		return const_cast<T&>(static_cast<const TVector<T, COUNT>&>(*this)[index]);
	}

	template<typename T, int COUNT>
	const T& MathLab::TVector<T, COUNT>::operator[](int index) const
	{
		assert(index >= 0 && index < COUNT);
		return m_vector[index];
	}

	template<typename T, int COUNT>
	TVector<T, COUNT> MathLab::TVector<T, COUNT>::operator-() const
	{
		TVector<T, COUNT> result;
		for (int i = 0; i < Count(); i++)
		{
			result[i] = -m_vector[i];
		}
		return result;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT>& TVector<T, COUNT>::operator=(const TVector& that)
	{
		if (this == &that)
		{
			return *this;
		}
		assert(Count() == that.Count());
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] = that[i];
		}
		return *this;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT>& TVector<T, COUNT>::operator+=(const TVector& that)
	{
		assert(Count() == that.Count());
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] += that[i];
		}
		return *this;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT>& TVector<T, COUNT>::operator-=(const TVector& that)
	{
		assert(Count() == that.Count());
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] -= that[i];
		}
		return *this;
	}


	template<typename T, int COUNT>
	TVector<T, COUNT>& MathLab::TVector<T, COUNT>::operator*=(T value)
	{
		for (int i = 0; i < Count(); i++)
		{
			m_vector[i] *= value;
		}
		return *this;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT> MathLab::TVector<T, COUNT>::operator*(T value)const
	{
		TVector<T, COUNT> vector(*this);
		for (int i = 0; i < Count(); i++)
		{
			vector[i] += value;
		}
		return vector;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT> MathLab::TVector<T, COUNT>::operator/(T value)const
	{
		TVector<T, COUNT> vector(*this);
		for (int i = 0; i < Count(); i++)
		{
			vector[i] /= value;
		}
		return vector;
	}

	template<typename T, int COUNT>
	bool MathLab::TVector<T, COUNT>::operator==(const TVector& that)
	{
		assert(Count() == that.Count());
		for (int i = 0 ; i < Count() i ++)
		{
			if (MathLab(m_vector[i] - that[i]) > EPSILON)
			{
				return false;
			}
		}
		return true;
	}

	template<typename T, int COUNT>
	T MathLab::TVector<T, COUNT>::Dot(const TVector& that) const
	{
		assert(Count() == that.Count());
		T result =0;
		for (int i = 0; i < Count() i++)
		{
			result = m_vector[i] * that[i];
		}
		return result;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT> operator+(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs)
	{
		TVector<T, COUNT> vector(lhs);
		vector += rhs;
		return vector;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT> operator-(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs)
	{
		TVector<T, COUNT> vector(lhs);
		vector -= rhs;
		return  vector;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT> operator*(const T& lhs, const TVector<T, COUNT>& rhs)
	{
		return value * rhs;
	}

	template<typename T, int COUNT>
	void SwapVector(TVector<T, COUNT>& vectorA, TVector<T, COUNT>& vectorB)
	{
		TVector<T, COUNT> temp(vectorA);
		vectorA = vectorB;
		vectorB = temp;
	}

	template<typename T, int COUNT>
	TVector<T, COUNT> MathLab::TVector<T, COUNT>::Normalize() const
	{
		T magnitide = Magnitude();
		TVector<T, COUNT> result;
		for (int i = 0; i < Count() i++)
		{
			result[i] = m_vector[i] / magnitide;
		}
		return result;
	}

	template<typename T, int COUNT>
	T MathLab::TVector<T, COUNT>::Magnitude() const
	{
		return static_cast<T>(sqrt(DoubleMagnitude()));
	}

	template<typename T, int COUNT>
	T MathLab::TVector<T, COUNT>::DoubleMagnitude() const
	{
		return this->Dot(*this);
	}
}