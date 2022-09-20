#pragma once
//#include "MathLab/Vector3.h"
//#include "MathLab/Vector2.h"
//#include "CrossPlatform/SColorRGBA.h"
//#include "crossguid/guid.hpp"
#include "Algorithms/Vector.h"
//#include "CrossPlatform/Polygon/Triangle3D.h"

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
	//using MathLab::Vector3f;
	//using MathLab::Vector2f;
	//using CrossPlatform::SColorRGBA;

	class Polygon3D
	{
	public:
		template<> friend struct YAML::convert<Polygon3D>;

        struct VertexIndex
        {
		public:
            int m_vertexIndex = -1;
            int m_normalIndex = -1;
            int m_textureCoordinateIndex = -1;
        };
		
		struct TriangleIndex
		{
		public:
			const VertexIndex& operator[](int index)const { assert(index >= 0 && index < 3); return m_vertex[index]; }
			VertexIndex& operator[](int index) { return const_cast<VertexIndex&>(static_cast<const TriangleIndex&>(*this)[index]); }

			VertexIndex m_vertex[3];
			int m_materialIndex = -1;
		};

		Polygon3D(Algorithm::Vector<TriangleIndex>&& vertexIndex);
		Polygon3D(const Polygon3D& that);
		~Polygon3D();

		int Count()const { return m_triangle.Count(); }
		const TriangleIndex& operator[](int index)const { return m_triangle[index]; }

        //const Vector3f& GetNormal(int index) const;
  //      int GetNumOfVertex()const { return m_numOfVertex; }
  //      int GetNumOfNormal()const { return m_numOfNormal; }
		//int GetNumOFUV()const { return m_numOfTextureCoordinate; }
		//const xg::Guid& GetModelGUID()const { return m_modelId; }
		//void SetModelGUID(const xg::Guid& guid) { m_modelId = guid; }
	private:
		//xg::Guid m_modelId;
		Algorithm::Vector<TriangleIndex> m_triangle;
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