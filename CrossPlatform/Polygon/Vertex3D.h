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
		int m_materialIndex = -1;
	};
} 