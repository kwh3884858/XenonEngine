//  Matrix.h
//  XenonEngine
//
//  Created by whkong on 2021-9-3.
//  Copyright (c) 2021 whkong. All rights reserved.

#pragma once
#include "Vector3.h"
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

	private:
		T* m_content;

	};

	template<typename T, int ROW, int COLUMN>
	Matrix<T, 1, COLUMN> operator*(const Matrix<T,1, ROW>& lValue, const Matrix<T, ROW, COLUMN>& rValue);

	template<typename T, int ROW, int COLUMN>
	MathLab::Matrix<T, ROW, COLUMN>::Matrix()
	{
		m_content = new T[ROW*COLUMN];
	}

	template<typename T, int ROW, int COLUMN>
	MathLab::Matrix<T, ROW, COLUMN>::Matrix(const Matrix& that)
	{
		Matrix();
		for (int i = 0; i < ROW*COLUMN; i++)
		{
			m_content[i] = that[i];
		}
	}

	template<typename T, int ROW, int COLUMN>
	MathLab::Matrix<T, ROW, COLUMN>::Matrix(std::initializer_list<T> param)
	{
		Matrix();
		for (int i =0;i < ROW*COLUMN; i++)
		{
			m_content[i] = param[i];
		}
	}

	template<typename T, int ROW, int COLUMN>
	Matrix<T, ROW, COLUMN>::~Matrix()
	{
		delete[] m_content;
		m_content = nullptr;
	}

	template<typename T, int ROW, int COLUMN>
	Vector3<T> operator*(const Vector3<T>& lValue, const Matrix<T, ROW, COLUMN>& rValue)
	{
		Vector3f result;
		result.x = lValue.x * rValue.v1.x + lValue.y * rValue.v2.x + lValue.z * rValue.v3.x;
		result.y = lValue.x * rValue.v1.y + lValue.y * rValue.v2.y + lValue.z * rValue.v3.y;
		result.z = lValue.x * rValue.v1.z + lValue.y * rValue.v2.z + lValue.z * rValue.v3.z;
		return result;
	}

	template<typename T, int ROW, int COLUMN>
	Matrix<T, 1, COLUMN> operator*(const Matrix<T, 1, ROW>& lValue, const Matrix<T, ROW, COLUMN>& rValue)
	{
		Matrix<T, 1, COLUMN> result;
		for (int i = 0; i < COLUMN; i++)
		{
			for (int j = 0; j < ROW; j++)
			{
			}
		}
	}

}