#pragma once

#include "MathLab/Vector3.h"
#include "MathLab/Vector2.h"

namespace CrossPlatform {
	using MathLab::Vector3f;
	using MathLab::Vector2f;

	class Vertex3D
	{
	public:
		Vertex3D(const Vector3f& vertex, const Vector3f& normal, const Vector2f& uv, int material) { m_vertex = vertex; m_normal = normal; m_uv = uv; }
		Vector3f m_vertex;
		Vector3f m_color;
		Vector3f m_normal;
		Vector2f m_uv;
	};
}