#pragma once

namespace MathLab
{
	template<typename T, int COUNT>
	struct Vector;

	template<typename T, int COUNT>
	Vector<T, COUNT> operator+(const Vector<T, COUNT>& lhs, const Vector<T, COUNT>& rhs);
	template<typename T, int COUNT>
	Vector<T, COUNT> operator-(const Vector<T, COUNT>& lhs, const Vector<T, COUNT>& rhs);
	template<typename T, int COUNT>
	Vector<T, COUNT> operator*(const T& lhs, const Vector<T, COUNT>& value);

	template<typename T, int COUNT>
	void SwapVector(Vector<T, COUNT>& vectorA, Vector<T, COUNT>& vectorB);

	template<typename T, int COUNT>
	class Vector
	{
	public:
		T* m_vector;

	public:
		static Vector Zero;

		friend void SwapVector<T, COUNT>(Vector<T, COUNT>& vectorA, Vector<T, COUNT>& vectorB);

		friend Vector<T, COUNT> operator+ <>(const Vector<T, COUNT>& lhs, const Vector<T, COUNT>& rhs);
		friend Vector<T, COUNT> operator- <>(const Vector<T, COUNT>& lhs, const Vector<T, COUNT>& rhs);
		
		T& operator[](int index);
		const T& operator[](int index)const;
		Vector operator-()const;
		Vector& operator=(const Vector& that);
		Vector& operator+=(const Vector& that);
		Vector& operator-=(const Vector& that);
		Vector& operator*=(T value);

		Vector operator*(T value)const;
		Vector operator/(T value)const;

		bool operator==(const Vector& that);

		T Dot(const Vector& that)const;
		//T Cross(const Vector& that)const;

		Vector();
		Vector(const Vector& that);
		~Vector();
		
		inline int Count()const { return COUNT; }
		Vector Normalize()const;
		T Magnitude()const;
		T DoubleMagnitude()const;
	};

	template<typename T, int COUNT>
	__declspec(selectany) Vector<T, COUNT> Vector<T, COUNT>::Zero = Vector<T, COUNT>();


	template<typename T, int COUNT>
	Vector<T, COUNT>::Vector()
	{
		m_vector = new T[COUNT];
		memset(m_vector, 0, COUNT * sizeof(T));
	}

	template<typename T, int COUNT>
	Vector<T, COUNT>::Vector(const Vector& that)
	{
		Vector();
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] = that[i];
		}
	}

	template<typename T, int COUNT>
	Vector<T, COUNT>::~Vector()
	{
		delete[] m_vector;
		m_vector = nullptr;
	}

	template<typename T, int COUNT>
	T& MathLab::Vector<T, COUNT>::operator[](int index)
	{
		return const_cast<T&>(static_cast<const Vector<T, COUNT>&>(*this)[index]);
	}

	template<typename T, int COUNT>
	const T& MathLab::Vector<T, COUNT>::operator[](int index) const
	{
		assert(index >= 0 && index < COUNT);
		return m_vector[index];
	}

	template<typename T, int COUNT>
	Vector<T, COUNT> MathLab::Vector<T, COUNT>::operator-() const
	{
		Vector<T, COUNT> result;
		for (int i = 0; i < Count(); i++)
		{
			result[i] = -m_vector[i];
		}
		return result;
	}

	template<typename T, int COUNT>
	Vector<T, COUNT>& Vector<T, COUNT>::operator=(const Vector& that)
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
	Vector<T, COUNT>& Vector<T, COUNT>::operator+=(const Vector& that)
	{
		assert(Count() == that.Count());
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] += that[i];
		}
		return *this;
	}

	template<typename T, int COUNT>
	Vector<T, COUNT>& Vector<T, COUNT>::operator-=(const Vector& that)
	{
		assert(Count() == that.Count());
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] -= that[i];
		}
		return *this;
	}


	template<typename T, int COUNT>
	Vector<T, COUNT>& MathLab::Vector<T, COUNT>::operator*=(T value)
	{
		for (int i = 0; i < Count(); i++)
		{
			m_vector[i] *= value;
		}
		return *this;
	}

	template<typename T, int COUNT>
	Vector<T, COUNT> MathLab::Vector<T, COUNT>::operator*(T value)const
	{
		Vector<T, COUNT> vector(*this);
		for (int i = 0; i < Count(); i++)
		{
			vector[i] += value;
		}
		return vector;
	}

	template<typename T, int COUNT>
	Vector<T, COUNT> MathLab::Vector<T, COUNT>::operator/(T value)const
	{
		Vector<T, COUNT> vector(*this);
		for (int i = 0; i < Count(); i++)
		{
			vector[i] /= value;
		}
		return vector;
	}

	template<typename T, int COUNT>
	bool MathLab::Vector<T, COUNT>::operator==(const Vector& that)
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
	T MathLab::Vector<T, COUNT>::Dot(const Vector& that) const
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
	Vector<T, COUNT> operator+(const Vector<T, COUNT>& lhs, const Vector<T, COUNT>& rhs)
	{
		Vector<T, COUNT> vector(lhs);
		vector += rhs;
		return vector;
	}

	template<typename T, int COUNT>
	Vector<T, COUNT> operator-(const Vector<T, COUNT>& lhs, const Vector<T, COUNT>& rhs)
	{
		Vector<T, COUNT> vector(lhs);
		vector -= rhs;
		return  vector;
	}

	template<typename T, int COUNT>
	Vector<T, COUNT> operator*(const T& lhs, const Vector<T, COUNT>& rhs)
	{
		return value * rhs;
	}

	template<typename T, int COUNT>
	void SwapVector(Vector<T, COUNT>& vectorA, Vector<T, COUNT>& vectorB)
	{
		Vector<T, COUNT> temp(vectorA);
		vectorA = vectorB;
		vectorB = temp;
	}

	template<typename T, int COUNT>
	Vector<T, COUNT> MathLab::Vector<T, COUNT>::Normalize() const
	{
		T magnitide = Magnitude();
		Vector<T, COUNT> result;
		for (int i = 0; i < Count() i++)
		{
			result[i] = m_vector[i] / magnitide;
		}
		return result;
	}

	template<typename T, int COUNT>
	T MathLab::Vector<T, COUNT>::Magnitude() const
	{
		return static_cast<T>(sqrt(DoubleMagnitude()));
	}

	template<typename T, int COUNT>
	T MathLab::Vector<T, COUNT>::DoubleMagnitude() const
	{
		return this->Dot(*this);
	}
}