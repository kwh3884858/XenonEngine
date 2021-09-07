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
	struct TMatrix;

	typedef TMatrix<float, 4, 4> TMatrix4X4f;
	typedef TMatrix<float, 4, 3> TMatrix4X3f;
	typedef TMatrix<float, 3, 3> TMatrix3X3f;

	template<typename T, int ROW, int COLUMN>
	struct TMatrix final {

	public:
		TMatrix();
		TMatrix(std::initializer_list<T> param);
		TMatrix(const TMatrix& that);
		TMatrix(const TMatrix<T, ROW-1, COLUMN>& that);
		TMatrix(const TMatrix<T, ROW-1, COLUMN-1>& that);
		~TMatrix();

		T& operator[](int index);
		const T& operator[](int index)const;
	private:
		TVector<T,COLUMN>* m_matrix;
	};

	template<typename T, int ROW, int COLUMN>
	TVector<T, COLUMN> operator*(const TVector<T, ROW>& lValue, const TMatrix<T, ROW, COLUMN>& rValue);

	template<typename T, int ROW, int COLUMN>
	MathLab::TMatrix<T, ROW, COLUMN>::TMatrix()
	{
		m_content = new T[ROW];
	}

	template<typename T, int ROW, int COLUMN>
	MathLab::TMatrix<T, ROW, COLUMN>::TMatrix(const TMatrix& that)
	{
		TMatrix();
		for (int i = 0; i < ROW; i++)
		{
			(*this)[i] = that[i];
		}
	}

	template<typename T, int ROW, int COLUMN>
	MathLab::TMatrix<T, ROW, COLUMN>::TMatrix(const TMatrix<T, ROW - 1, COLUMN>& that)
	{
		TMatrix();
		for (int i = 0; i < ROW - 1; i++)
		{
			(*this)[i] = that[i];
		}
		TVector<T, COLUMN> unit;
		unit[COLUMN - 1] = 1;
		(*this)[ROW - 1] = unit;
	}

	template<typename T, int ROW, int COLUMN>
	MathLab::TMatrix<T, ROW, COLUMN>::TMatrix(std::initializer_list<T> param)
	{
		assert(param.size() == ROW * COLUMN);
		TMatrix();
		for (int i = 0;i < ROW*COLUMN; i++)
		{
			m_content[i] = param[i];
		}
	}

	template<typename T, int ROW, int COLUMN>
	TMatrix<T, ROW, COLUMN>::~TMatrix()
	{
		delete[] m_matrix;
		m_matrix = nullptr;
	}

	template<typename T, int ROW, int COLUMN>
	T& MathLab::TMatrix<T, ROW, COLUMN>::operator[](int index)
	{
		return const_cast<T&>(static_cast<const TMatrix<T, ROW, COLUMN>&>(*this)[index]);
	}

	template<typename T, int ROW, int COLUMN>
	const T& MathLab::TMatrix<T, ROW, COLUMN>::operator[](int index) const
	{
		assert(index >= 0 && index < ROW* COLUMN);
		int row = index / COLUMN;
		int column = index % COLUMN;
		return m_matrix[row][column];
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
	TVector<T, COLUMN> operator*(const TVector<T, ROW>& lValue, const TMatrix<T, ROW, COLUMN>& rValue)
	{
		TVector<T, COLUMN> result;
		for (int i = 0; i < COLUMN; i++)
		{
			for (int j = 0; j < ROW; j++)
			{
				result[i] += lValue[j] * rValue[j][i];
			}
		}
	}

}