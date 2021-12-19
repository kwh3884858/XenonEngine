#include "Polygon3D.h"
#include <cassert>

namespace CrossPlatform 
{
	Polygon3D::Polygon3D(int numOfIndex, VertexIndexs* vertexIndexList, int numOfVertex, Vector3f * vertexList, int numOfNormal, Vector3f* normalList, int numofCoordinate /*= 0*/, Vector2f* textureCoordinate /*= nullptr*/, int numOfMaterialIndex /*= 0*/, int* materialIndex /*= nullptr*/):
		m_numOfIndex(numOfIndex),
		m_vertexIndexList(vertexIndexList),
		m_numOfVertex(numOfVertex),
		m_vertexList(vertexList),
        m_numOfNormal(numOfNormal),
        m_normalList(normalList),
		m_numOfTextureCoordinate(numofCoordinate),
		m_textureCoordinate(textureCoordinate),
		m_numOfMaterialIndex(numOfMaterialIndex),
		m_materialIndex(materialIndex)
	{
	}

	Polygon3D::Polygon3D(const Polygon3D& that)
	{
		m_modelId = that.m_modelId;
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
		m_normalList = new Vector3f[m_numOfNormal];
		for (int i = 0; i < m_numOfNormal; i++)
		{
			m_normalList[i] = that.m_normalList[i];
		}
		m_numOfTextureCoordinate = that.m_numOfTextureCoordinate;
		m_textureCoordinate = new Vector2f[m_numOfTextureCoordinate];
		for (int i = 0; i < m_numOfTextureCoordinate; i++)
		{
			m_textureCoordinate[i] = that.m_textureCoordinate[i];
		}
		m_numOfMaterialIndex = that.m_numOfMaterialIndex;
		m_materialIndex = new int[m_numOfMaterialIndex];
		for (int i = 0; i < m_numOfMaterialIndex; i++)
		{
			m_materialIndex[i] = that.m_materialIndex[i];
		}
	}

	const Vertex3D Polygon3D::operator[](int index) const
	{
		assert(m_vertexIndexList != nullptr);
		assert(m_vertexList != nullptr);
		assert(m_numOfNormal == 0 ? m_normalList == nullptr : m_normalList != nullptr);
		assert(m_numOfTextureCoordinate == 0? m_textureCoordinate == nullptr: m_textureCoordinate != nullptr);
		assert(index >= 0 && index < m_numOfIndex);
        VertexIndexs vertexIndex = m_vertexIndexList[index];
		assert(vertexIndex.m_vertexIndex >= 0 && vertexIndex.m_vertexIndex < m_numOfVertex);
		Vector3f normal = Vector3f(0, 0, 0);
		if (m_numOfNormal > 0)
		{
			normal = m_normalList[vertexIndex.m_normalIndex];
		}
		Vector2f uv = Vector2f(-1, -1);
		if (m_numOfTextureCoordinate > 0)
		{
			uv = m_textureCoordinate[vertexIndex.m_textureCoordinateIndex];
		}
		int material = -1;
		if (m_numOfMaterialIndex > 0)
		{
			material = m_materialIndex[index / 3];
		}
		Vertex3D result(m_vertexList[vertexIndex.m_vertexIndex], normal, uv, material);
		return result;
	}

    //const Vector3f& Polygon3D::GetNormal(int index) const
    //{
    //    assert(m_normalList != nullptr);
    //    assert(index >= 0 && index < m_numOfIndex);
    //    VertexIndexs vertexIndex = m_vertexIndexList[index];
    //    assert(vertexIndex.m_normalIndex >= 0 && vertexIndex.m_normalIndex < m_numOfNormal);

    //    return m_normalList[vertexIndex.m_normalIndex];
    //}

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
		delete[] m_textureCoordinate;
		m_textureCoordinate = nullptr;
		delete[] m_materialIndex;
		m_materialIndex = nullptr;

		m_numOfIndex = 0; 
		m_numOfVertex = 0; 
		m_numOfNormal = 0; 
		m_numOfTextureCoordinate = 0; 
		m_numOfMaterialIndex = 0;
	}

}