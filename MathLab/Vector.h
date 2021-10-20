#pragma once
#include <cassert>
#include <Algorithms/Vector.h>
#include <initializer_list>

namespace MathLab
{
    using Algorithm::Vector;

	template<typename T, int COUNT>
	struct TVector;

    typedef     TVector<float, 4>      TVector4f;
    typedef     TVector<float, 3>      TVector3f;

	template<typename T, int COUNT>
	TVector<T, COUNT> operator+(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs);
	template<typename T, int COUNT>
	TVector<T, COUNT> operator-(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs);
	template<typename T, int COUNT>
	TVector<T, COUNT> operator*(const T& lhs, const TVector<T, COUNT>& value);
    template<typename T, int COUNT>
    T operator*(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs);

    template<typename T, int COUNT>
	void SwapVector(TVector<T, COUNT>& vectorA, TVector<T, COUNT>& vectorB);

	template<typename T, int COUNT>
	class TVector
	{
	private:
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

		TVector();
        TVector(const TVector& that);
        TVector(std::initializer_list<T>& param);
        TVector(const Vector<T>& vector);
        TVector(const Vector3f& vector3f);
		~TVector();
		
		inline int Count()const { return COUNT; }
		TVector Normalize()const;
		T Magnitude()const;
		T DoubleMagnitude()const;
        Vector3<T> GetVetor();

		T Dot(const TVector& vec)const;
		TVector Cross(const TVector& vec)const;
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
	TVector<T, COUNT>::TVector(const TVector& that):TVector()
	{
		for (int i = 0; i < that.Count(); i++)
		{
			m_vector[i] = that[i];
		}
	}

    template<typename T, int COUNT>
    MathLab::TVector<T, COUNT>::TVector(std::initializer_list<T>& param):TVector()
    {
        assert(param.size() == COUNT);
        std::initializer_list<T>::const_iterator iter = param.begin();
        for (int i = 0; i < COUNT; i++)
        {
            (*this)[i] = *iter;
            iter++;
        }
    }

    template<typename T, int COUNT>
    MathLab::TVector<T, COUNT>::TVector(const Vector<T>& vector) :TVector()
    {
        assert(COUNT >= vector.Count());
        for (int i = 0; i < vector.Count(); i++)
        {
            m_vector[i] = vector[i];
        }
    }

    template<typename T, int COUNT>
    MathLab::TVector<T, COUNT>::TVector(const Vector3f& vector3f) :TVector()
    {
        assert(COUNT >= 3);
        m_vector[0] = vector3f.x;
        m_vector[1] = vector3f.y;
        m_vector[2] = vector3f.z;
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
    T operator*(const TVector<T, COUNT>& lhs, const TVector<T, COUNT>& rhs)
    {
        T result = 0;
        for (int i = 0; i < COUNT; i++)
        {
            result += lhs[i] * rhs[i];
        }
        return result;
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
        assert(COUNT == 3 || COUNT == 4);
		T magnitide = Magnitude();
		TVector<T, COUNT> result(*this);
		for (int i = 0; i < 3; i++)
		{
			result[i] /= magnitide;
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
        assert(COUNT == 3 || COUNT == 4);
		return this->Dot(*this);
	}

    template<typename T, int COUNT>
    MathLab::Vector3<T> MathLab::TVector<T, COUNT>::GetVetor()
    {
        return Vector3<T>(m_vector[0], m_vector[1], m_vector[2]);
    }

	template<typename T, int COUNT>
	inline T TVector<T, COUNT>::Dot(const TVector& vec) const
	{
        assert(COUNT == 3 || COUNT == 4);
        assert(COUNT == vec.Count());
		float result = 0;
		for (size_t i = 0; i < 3; i++)
		{
			result += m_vector[i] * vec[i];
		}
		return result;
	}

	template<typename T, int COUNT>
	inline TVector<T, COUNT> TVector<T, COUNT>::Cross(const TVector& vec) const
	{
		assert(COUNT == 3 || COUNT == 4);
		TVector result(
			std::initializer_list<T>{
				m_vector[1]* vec[2] - m_vector[2] * vec[1],
				m_vector[2]* vec[0] - m_vector[0] * vec[2],
				m_vector[0]* vec[1] - m_vector[1] * vec[0],
				1});
		return result;
	}

}