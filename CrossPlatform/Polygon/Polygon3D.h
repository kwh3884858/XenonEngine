#pragma once
#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"
#include "CrossPlatform/SColorRGBA.h"
#include "crossguid/guid.hpp"
#include "Algorithms/Vector.h"
#include "CrossPlatform/Polygon/Vertex3D.h"

namespace CrossPlatform {
	using MathLab::Vector3f;
	using MathLab::Vector2f;
	using CrossPlatform::SColorRGBA;

	class Polygon3D
	{
	public:
        struct VertexIndexs
        {
            int m_vertexIndex = -1;
            int m_normalIndex = -1;
            int m_textureCoordinateIndex = -1;
        };
		Polygon3D(int numOfPtr, VertexIndexs* vertexPointerList, int numOfVertex, Vector3f* vertexList,int numOfNormal, Vector3f* normalList, int numofCoordinate = 0, Vector2f* textureCoordinate = nullptr, int numOfMaterialIndex = 0, int* materialIndex = nullptr);
		Polygon3D(const Polygon3D& that);

		int Count()const { return m_numOfIndex; }
		const Vertex3D operator[](int index)const;
        //const Vector3f& GetNormal(int index) const;
		~Polygon3D();

        int GetNumOfVertex()const { return m_numOfVertex; }
        int GetNumOfNormal()const { return m_numOfNormal; }
		int GetNumOFUV()const { return m_numOfTextureCoordinate; }
		const xg::Guid& GetModelGUID()const { return m_modelId; }
		void SetModelGUID(const xg::Guid& guid) { m_modelId = guid; }
	private:
		xg::Guid m_modelId;
		int m_numOfIndex = 0;
        VertexIndexs* m_vertexIndexList = nullptr;
		int m_numOfVertex = 0;
		Vector3f* m_vertexList = nullptr;
        int m_numOfNormal = 0;
        Vector3f* m_normalList = nullptr;
		int m_numOfTextureCoordinate = 0;
		Vector2f* m_textureCoordinate = nullptr;
		int m_numOfMaterialIndex = 0;
		int* m_materialIndex = nullptr;
	};
}