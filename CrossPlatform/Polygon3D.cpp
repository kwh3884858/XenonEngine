#include "Polygon3D.h"
#include <cassert>

namespace CrossPlatform 
{
	Polygon3D::Polygon3D(int numOfIndex, int* vertexIndexList, int numOfVertex, Vector3f * vertexList):
		m_numOfIndex(numOfIndex),
		m_vertexIndexList(vertexIndexList),
		m_numOfVertex(numOfVertex),
		m_vertexList(vertexList)
	{
	}

	Polygon3D::Polygon3D(const Polygon3D& that)
	{
		m_numOfIndex = that.m_numOfIndex;
		m_vertexIndexList = that.m_vertexIndexList;
		m_numOfVertex = that.m_numOfVertex;
		m_vertexList = that.m_vertexList;
	}

	const Vector3f& Polygon3D::operator[](int index) const
	{
		assert(m_vertexIndexList != nullptr);
		assert(m_vertexList != nullptr);
		assert(index >= 0 && index < m_numOfIndex);
		int vertexIndex = m_vertexIndexList[index];
		assert(vertexIndex >= 0 && vertexIndex < m_numOfVertex);

		return m_vertexList[vertexIndex];
	}

	Polygon3D::~Polygon3D()
	{
		assert(m_vertexIndexList != nullptr);
		assert(m_vertexList != nullptr);
		delete[] m_vertexIndexList;
		m_vertexIndexList = nullptr;
		delete[] m_vertexList;
		m_vertexList = nullptr;
	}

}