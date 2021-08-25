#include "Polygon3D.h"
#include <cassert>

namespace CrossPlatform 
{
	Polygon3D::Polygon3D(SColorRGBA color, int numOfPtr, Polygon3DPointer* vertexPointerList, int numOfVertex, Polygon3DVertex* vertexList):
		m_color(color),
		m_numOfPtr(numOfPtr),
		m_vertexPointerList(vertexPointerList),
		m_numOfVertex(numOfVertex),
		m_vertexList(vertexList)
	{
	}

	Polygon3D::Polygon3D(const Polygon3D& that)
	{
		m_color = that.m_color;
		m_numOfPtr = that.m_numOfPtr;
		m_vertexPointerList = that.m_vertexPointerList;
		m_numOfVertex = that.m_numOfVertex;
		m_vertexList = that.m_vertexList;
	}

	Polygon3D::~Polygon3D()
	{
		assert(m_vertexPointerList != nullptr);
		assert(m_vertexList != nullptr);
		delete[] m_vertexPointerList;
		m_vertexPointerList = nullptr;
		delete[] m_vertexList;
		m_vertexList = nullptr;
	}

}