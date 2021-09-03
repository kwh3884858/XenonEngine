#pragma once

namespace MathLab
{
	template<typename T, int COUNT>
	class Vector
	{
	public:
		T* m_content;

	public:
		static Vector Zero;

		friend void SwapVector<T>(Vector<T>& vectorA, Vector<T>& vectorB);
		friend bool LessY<T>(const Vector<T>& origin, const Vector<T>& compare);

		friend Vector<T> operator+ <>(const Vector<T>& v1, const Vector<T>& v2);
		friend Vector<T> operator- <>(const Vector<T>& v1, const Vector<T>& v2);

		Vector operator-()const;
		Vector& operator=(const Vector& value);
		Vector& operator+=(const Vector& value);
		Vector& operator-=(const Vector& value);
		Vector& operator*=(T value);

		Vector operator*(T value)const;
		Vector operator/(T value)const;

		bool operator==(const Vector& value);

		T Dot(const Vector& vec)const;
		T Cross(const Vector& vec)const;

		Vector();
		Vector(T ax, T ay);
		Vector(const Vector&);
		~Vector();

		void Swap();
		Vector Normalize()const;
		T Magnitude()const;
		T DoubleMagnitude()const;
	};


	template<typename T>
	__declspec(selectany) Vector2<T> Vector2<T>::Zero = Vector2f(0, 0);


	template<typename T>
	Vector2<T>::Vector2()
	{
		x = 0;
		y = 0;
	}

	template<typename T>
	Vector2<T>::Vector2(T ax, T ay)
	{
		x = ax;
		y = ay;
	}

	template<typename T>
	Vector2<T>::Vector2(const Vector2& para)
	{
		this->x = para.x;
		this->y = para.y;
	}

	template<typename T>
	Vector2<T>::~Vector2()
	{
	}

	template<typename T>
	Vector2<T> MathLab::Vector2<T>::operator-() const
	{
		Vector2 result(-this->x, -this->y);
		return result;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator=(const Vector2& value)
	{
		if (this == &value)
		{
			return *this;
		}
		this->x = value.x;
		this->y = value.y;
		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator+=(const Vector2& value)
	{
		this->x += value.x;
		this->y += value.y;
		return *this;
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator-=(const Vector2& value)
	{
		this->x -= value.x;
		this->y -= value.y;
		return *this;
	}


	template<typename T>
	Vector2<T>& MathLab::Vector2<T>::operator*=(T value)
	{
		this->x *= value;
		this->y *= value;
		return *this;
	}

	template<typename T>
	Vector2<T> MathLab::Vector2<T>::operator*(T value)const
	{
		Vector2<T> vector(*this);
		vector *= value;
		return vector;
	}

	template<typename T>
	Vector2<T> MathLab::Vector2<T>::operator/(T value)const
	{
		Vector2<T> vector(*this);
		vector.x /= value;
		vector.y /= value;
		return vector;
	}

	template<typename T>
	bool MathLab::Vector2<T>::operator==(const Vector2& value)
	{
		return MathLab(this->x - value.x) < EPSILON && MathLab(this->y - value.y) < EPSILON;
	}

	template<typename T>
	T MathLab::Vector2<T>::Dot(const Vector2& vec) const
	{
		return this->x * vec.x + this->y * vec.y;
	}

	template<typename T>
	T MathLab::Vector2<T>::Cross(const Vector2& vec) const
	{
		return this->x * vec.y - vec.x * this->y;
	}

	template<typename T>
	void Vector2<T>::Swap() {
		T temp = x;
		x = y;
		y = temp;
	}

	template<typename T>
	Vector2<T> operator+(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> vector(v1);
		vector += v2;
		return  vector;
	}

	template<typename T>
	Vector2<T> operator-(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> vector(v1);
		vector -= v2;
		return  vector;
	}

	template<typename T>
	Vector2<T> operator*(const T& v1, const Vector2<T>& value)
	{
		return value * v1;
	}

	template<typename T>
	void SwapVector(Vector2<T>& vectorA, Vector2<T>& vectorB)
	{
		Vector2<T> temp(vectorA);
		vectorA = vectorB;
		vectorB = temp;
	}

	template<typename T>
	bool LessY(const Vector2<T>& origin, const Vector2<T>& compare)
	{
		return origin.y < compare.y;
	}

	template<typename T>
	Vector2<T> MathLab::Vector2<T>::Normalize() const
	{
		T magnitide = Magnitude();
		Vector2<T> temp;
		temp.x = this->x / magnitide;
		temp.y = this->y / magnitide;
		return temp;
	}

	template<typename T>
	T MathLab::Vector2<T>::Magnitude() const
	{
		return static_cast<T>(sqrt(DoubleMagnitude()));
	}

	template<typename T>
	T MathLab::Vector2<T>::DoubleMagnitude() const
	{
		return this->Dot(*this);
	}
}