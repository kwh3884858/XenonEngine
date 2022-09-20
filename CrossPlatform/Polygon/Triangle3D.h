#pragma once

#include "MathLab/Vector.h"
#include "MathLab/Matrix.h"
#include "MathLab/Vector2.h"

namespace CrossPlatform {
	using MathLab::TVector4f;
	using MathLab::TMatrix4X4f;
	using MathLab::Vector2f;

	class Vertex3D
	{
	public:
		TVector4f m_vertex;
		TVector4f m_normal;
		Vector2f m_uv;
	};

	class Triangle3D
	{
	public:
		const CrossPlatform::Vertex3D& operator[](int index)const { assert(index >= 0 && index < 3); return m_vertex[index]; }
		CrossPlatform::Vertex3D& operator[](int index) { return const_cast<CrossPlatform::Vertex3D&>(static_cast<const Triangle3D&>(*this)[index]); }

		Vertex3D m_vertex[3];
		int m_materialIndex = -1;
	};
} 