//  Matrix.h
//  XenonEngine
//
//  Created by whkong on 2021-9-3.
//  Copyright (c) 2021 whkong. All rights reserved.

#pragma once
#include "Vector.h"
#include <initializer_list>

namespace MathLab {
	template<typename T, int ROW, int COLUMN>
	struct Matrix;


	template<typename T, int ROW, int COLUMN>
	struct Matrix final {

	public:
		Matrix();
		Matrix(std::initializer_list<T> param);
		Matrix(const Matrix& that);
		~Matrix();

		T& operator[](int index);
		const T& operator[](int index)const;
	private:
		Vector<T,COLUMN>* m_matrix;
	};

	template<typename T, int ROW, int COLUMN>
	Vector<T, COLUMN> operator*(const Vector<T, ROW>& lValue, const Matrix<T, ROW, COLUMN>& rValue);

	template<typename T, int ROW, int COLUMN>
	MathLab::Matrix<T, ROW, COLUMN>::Matrix()
	{
		m_content = new T[ROW];
	}

	template<typename T, int ROW, int COLUMN>
	MathLab::Matrix<T, ROW, COLUMN>::Matrix(const Matrix& that)
	{
		Matrix();
		for (int i = 0; i < ROW; i++)
		{
			(*this)[i] = that[i];
		}
	}

	template<typename T, int ROW, int COLUMN>
	MathLab::Matrix<T, ROW, COLUMN>::Matrix(std::initializer_list<T> param)
	{
		Matrix();
		for (int i = 0;i < ROW*COLUMN; i++)
		{
			m_content[i] = param[i];
		}
	}

	template<typename T, int ROW, int COLUMN>
	Matrix<T, ROW, COLUMN>::~Matrix()
	{
		delete[] m_matrix;
		m_matrix = nullptr;
	}

	template<typename T, int ROW, int COLUMN>
	T& MathLab::Matrix<T, ROW, COLUMN>::operator[](int index)
	{
		return const_cast<T&>(static_cast<const Matrix<T, COUNT, COLUMN>&>(*this)[index]);
	}

	template<typename T, int ROW, int COLUMN>
	const T& MathLab::Matrix<T, ROW, COLUMN>::operator[](int index) const
	{
		assert(index >= 0 && index < COUNT);
		return m_vector[index];
	}

	//template<typename T, int ROW, int COLUMN>
	//Vector3<T> operator*(const Vector3<T>& lValue, const Matrix<T, ROW, COLUMN>& rValue)
	//{
	//	Vector3f result;
	//	result.x = lValue.x * rValue.v1.x + lValue.y * rValue.v2.x + lValue.z * rValue.v3.x;
	//	result.y = lValue.x * rValue.v1.y + lValue.y * rValue.v2.y + lValue.z * rValue.v3.y;
	//	result.z = lValue.x * rValue.v1.z + lValue.y * rValue.v2.z + lValue.z * rValue.v3.z;
	//	return result;
	//}

	//template<typename T, int ROW, int COLUMN>
	//Matrix<T, 1, COLUMN> operator*(const Matrix<T, 1, ROW>& lValue, const Matrix<T, ROW, COLUMN>& rValue)
	//{
	//	Matrix<T, 1, COLUMN> result;
	//	for (int i = 0; i < COLUMN; i++)
	//	{
	//		T tmp = 0;
	//		for (int j = 0; j < ROW; j++)
	//		{
	//			tmp += 
	//		}
	//		result[0][i]
	//	}
	//}

	template<typename T, int ROW, int COLUMN>
	Vector<T, COLUMN> operator*(const Vector<T, ROW>& lValue, const Matrix<T, ROW, COLUMN>& rValue)
	{
		Vector<T, COLUMN> result;
		for (int i = 0; i < COLUMN; i++)
		{
			for (int j = 0; j < ROW; j++)
			{
				result[i] += lValue[j] * rValue[j][i];
			}
		}
	}

}