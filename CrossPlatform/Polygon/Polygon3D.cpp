#include "Polygon3D.h"
#include <cassert>

namespace CrossPlatform 
{
	Polygon3D::Polygon3D(int numOfIndex, VertexIndexs* vertexIndexList, int numOfVertex, Vector3f * vertexList, int numOfNormal, Vector3f* normalList):
		m_numOfIndex(numOfIndex),
		m_vertexIndexList(vertexIndexList),
		m_numOfVertex(numOfVertex),
		m_vertexList(vertexList),
        m_numOfNormal(numOfNormal),
        m_normalList(normalList)
	{
	}

	Polygon3D::Polygon3D(const Polygon3D& that)
	{
		m_numOfIndex = that.m_numOfIndex;
		m_vertexIndexList = new VertexIndexs[m_numOfIndex];
		for (int i = 0 ; i < m_numOfIndex; i++)
		{
			m_vertexIndexList[i] = that.m_vertexIndexList[i];
		}
		m_numOfVertex = that.m_numOfVertex;
		m_vertexList = new Vector3f[m_numOfVertex];
		for (int i = 0; i < m_numOfVertex; i++)
		{
			m_vertexList[i] = that.m_vertexList[i];
		}
        m_numOfNormal = that.m_numOfNormal;
        m_normalList = that.m_normalList;
		for (int i = 0; i < m_numOfNormal; i++)
		{
			m_normalList[i] = that.m_normalList[i];
		}
	}

	const Vertex3D Polygon3D::operator[](int index) const
	{
		assert(m_vertexIndexList != nullptr);
		assert(m_normalList != nullptr);
		assert(m_vertexList != nullptr);
		assert(index >= 0 && index < m_numOfIndex);
        VertexIndexs vertexIndex = m_vertexIndexList[index];
		assert(vertexIndex.m_vertexIndex >= 0 && vertexIndex.m_vertexIndex < m_numOfVertex);
		assert(vertexIndex.m_normalIndex >= 0 && vertexIndex.m_normalIndex < m_numOfNormal);

        Vertex3D result(m_vertexList[vertexIndex.m_vertexIndex], m_normalList[vertexIndex.m_normalIndex]);
		return result;
	}

    const Vector3f& Polygon3D::GetNormal(int index) const
    {
        assert(m_normalList != nullptr);
        assert(index >= 0 && index < m_numOfIndex);
        VertexIndexs vertexIndex = m_vertexIndexList[index];
        assert(vertexIndex.m_normalIndex >= 0 && vertexIndex.m_normalIndex < m_numOfNormal);

        return m_normalList[vertexIndex.m_normalIndex];
    }

    Polygon3D::~Polygon3D()
	{
		assert(m_vertexIndexList != nullptr);
		assert(m_vertexList != nullptr);
		delete[] m_vertexIndexList;
		m_vertexIndexList = nullptr;
		delete[] m_vertexList;
		m_vertexList = nullptr;
        delete[] m_normalList;
        m_normalList = nullptr;
	}

}