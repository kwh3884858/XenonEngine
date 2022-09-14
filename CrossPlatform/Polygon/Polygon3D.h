#pragma once
#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"
#include "CrossPlatform/SColorRGBA.h"
#include "crossguid/guid.hpp"
#include "Algorithms/Vector.h"
#include "CrossPlatform/Polygon/Vertex3D.h"

namespace CrossPlatform 
{
	class Polygon3D;
}

namespace YAML
{
	template<typename T> struct convert;
	template<> struct convert<CrossPlatform::Polygon3D>;
}

namespace CrossPlatform {
	using MathLab::Vector3f;
	using MathLab::Vector2f;
	using CrossPlatform::SColorRGBA;

	class Polygon3D
	{
	public:
		template<> friend struct YAML::convert<Polygon3D>;

        struct VertexIndexs
        {
            int m_vertexIndex = -1;
            int m_normalIndex = -1;
            int m_textureCoordinateIndex = -1;
			int m_materialIndex = -1;
        };
		Polygon3D(Algorithm::Vector<VertexIndexs>&& vertexIndex);
		Polygon3D(const Polygon3D& that);
		~Polygon3D();

		int Count()const { return m_vertexIndex.Count(); }
		const Vertex3D operator[](int index)const;

        //const Vector3f& GetNormal(int index) const;
  //      int GetNumOfVertex()const { return m_numOfVertex; }
  //      int GetNumOfNormal()const { return m_numOfNormal; }
		//int GetNumOFUV()const { return m_numOfTextureCoordinate; }
		//const xg::Guid& GetModelGUID()const { return m_modelId; }
		//void SetModelGUID(const xg::Guid& guid) { m_modelId = guid; }
	private:
		//xg::Guid m_modelId;
		Algorithm::Vector<VertexIndexs> m_vertexIndex;
		//int m_numOfIndex = 0;
  //      VertexIndexs* m_vertexIndexList = nullptr;
		//int m_numOfVertex = 0;
		//Vector3f* m_vertexList = nullptr;
  //      int m_numOfNormal = 0;
  //      Vector3f* m_normalList = nullptr;
		//int m_numOfTextureCoordinate = 0;
		//Vector2f* m_textureCoordinate = nullptr;
		//xg::Guid m_material;
	};
}