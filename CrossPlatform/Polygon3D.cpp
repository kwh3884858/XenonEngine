#include "Polygon3D.h"
#include <cassert>

namespace CrossPlatform 
{
	Polygon3D::Polygon3D(int numOfPtr, Polygon3DPointer* vertexPointerList, int numOfVertex, Polygon3DVertex* vertexList):
		m_numOfIndex(numOfPtr),
		m_vertexIndexList(vertexPointerList),
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