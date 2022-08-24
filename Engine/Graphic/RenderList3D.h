#pragma once

#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"
#include "Algorithms/Vector.h"
#include "CrossPlatform/Polygon/Vertex3D.h"
#include "CrossPlatform/Material/Material.h"

namespace XenonEngine {
	using MathLab::Vector3f;
	using MathLab::Vector2f;
	using CrossPlatform::Vertex3D;
	using CrossPlatform::Material;
	
	class Mesh3D;

	class RenderElement
	{
		Polygon3D* m_polygon;
		Material*
	};

	class RenderList3D
	{
		void AddMesh3D(const Mesh3D* const mesh);
		Algorithm::Vector<Polygon3D> m_vertexIndexList;
		Algorithm::Vector<Material> m_materials;
	};
}