#pragma once
#include "MathLab/Vector3.h"
#include "CrossPlatform/SColorRGBA.h"
#include "CrossPlatform/Polygon/Polygon3D.h"

namespace CrossPlatform {
	using MathLab::Vector3f;
	using CrossPlatform::SColorRGBA;

    class Vertex3D
    {
    public:
        Vertex3D(const Vector3f& vertex, const Vector3f& normal) { m_vertex = vertex; m_normal = normal; }
        Vector3f m_vertex;
        Vector3f m_normal;
    };

	class Polygon3D
	{
	public:
        struct VertexIndexs
        {
            int m_vertexIndex = -1;
            int m_normalIndex = -1;
            int m_textureCoordinateIndex = -1;
        };
		Polygon3D(int numOfPtr, VertexIndexs* vertexPointerList, int numOfVertex, Vector3f* vertexList,int numOfNormal, Vector3f* normalList);
		Polygon3D(const Polygon3D& that);

		int Count()const { return m_numOfIndex; }
		const Vertex3D operator[](int index)const;
        const Vector3f& GetNormal(int index) const;
		~Polygon3D();

        int GetNumOfVertex()const { return m_numOfVertex; }
        int GetNumOfNormal()const { return m_numOfNormal; }

	private:
		int m_numOfIndex = 0;
        VertexIndexs* m_vertexIndexList = nullptr;
		int m_numOfVertex = 0;
		Vector3f* m_vertexList = nullptr;
        int m_numOfNormal = 0;
        Vector3f* m_normalList = nullptr;
	};
}