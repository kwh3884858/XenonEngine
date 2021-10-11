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

        TVector<T, COLUMN> GetRow(int index)const;
        TVector<T, ROW> GetColumn(int index)const;

        TMatrix<T, ROW, COLUMN>& operator=(const TMatrix& rvalue);
        TMatrix<T, ROW, COLUMN>& operator*=(const TMatrix& rvalue);
	private:
		TVector<T,COLUMN>* m_matrix;
	};

	template<typename T, int ROW, int COLUMN>
	TVector<T, COLUMN> operator*(const TVector<T, ROW>& lValue, const TMatrix<T, ROW, COLUMN>& rValue);

    template<typename T, int ROW, int COLUMN>
    TMatrix<T, ROW, COLUMN> operator*(const TMatrix<T, ROW, COLUMN>& lValue, const TMatrix<T, ROW, COLUMN>& rValue);


	template<typename T, int ROW, int COLUMN>
	MathLab::TMatrix<T, ROW, COLUMN>::TMatrix()
	{
        m_matrix = new T[ROW];
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
        int i = 0;
        for (auto item: param)
        {
            m_matrix[i] = item;
            i++;
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

    template<typename T, int ROW, int COLUMN>
    MathLab::TVector<T, COLUMN> MathLab::TMatrix<T, ROW, COLUMN>::GetRow(int index) const
    {
        assert(index >= 0 && index < ROW);
        return m_matrix[index];
    }

    template<typename T, int ROW, int COLUMN>
    MathLab::TVector<T, ROW> MathLab::TMatrix<T, ROW, COLUMN>::GetColumn(int index) const
    {
        assert(index >= 0 && index < COLUMN);
        Vector<T> params;
        for (int i = 0; i < ROW; i++)
        {
            params.Add(m_matrix[i][index]);
        }
        MathLab::TVector<T, ROW> result(params);
        return result;
    }

    template<typename T, int ROW, int COLUMN>
    MathLab::TMatrix<T, ROW, COLUMN>& MathLab::TMatrix<T, ROW, COLUMN>::operator=(const TMatrix& rvalue)
    {
        if (this == &rvalue)
        {
            return *this;
        }
        for (int row = 0 ;row < ROW;row++)
        {
            m_matrix[row] = rvalue[row];
        }
        return *this;
    }

    template<typename T, int ROW, int COLUMN>
    MathLab::TMatrix<T, ROW, COLUMN>& TMatrix<T, ROW, COLUMN>::operator*=(const TMatrix& rvalue)
    {
        (*this) = (*this) * rvalue;
        return *this;
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

    template<typename T, int ROW, int COLUMN>
    TMatrix<T, ROW, COLUMN>
        operator*(const TMatrix<T, ROW, COLUMN>& lValue, const TMatrix<T, ROW, COLUMN>& rValue)
    {
        TMatrix<T, ROW, COLUMN> result;
        for (int i = 0; i < COLUMN; i++)
        {
            float portion =0;
            for (int j = 0; j < ROW; j++)
            {
                portion += lValue.GetRow(i) * rValue.GetColumn(j);
            }
            result[i] = portion;
        }
        return result;
    }

}