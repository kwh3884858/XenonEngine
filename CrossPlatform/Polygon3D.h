#pragma once
#include "MathLab/Vector3.h"
#include "CrossPlatform/SColorRGBA.h"

namespace CrossPlatform {
	using MathLab::Vector3f;
	using CrossPlatform::SColorRGBA;

	class Polygon3D
	{
	public:
		Polygon3D(int numOfPtr, int* vertexPointerList, int numOfVertex, Vector3f* vertexList);
		Polygon3D(const Polygon3D& that);

		int Count()const { return m_numOfIndex; }
		const Vector3f& operator[](int index)const;
		~Polygon3D();

        int GetNumOfVertex()const { return m_numOfVertex; }

	private:
		int m_numOfIndex = 0;
		int* m_vertexIndexList = nullptr;
		int m_numOfVertex = 0;
		Vector3f* m_vertexList = nullptr;
	};
}