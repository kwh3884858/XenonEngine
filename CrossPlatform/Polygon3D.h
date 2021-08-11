#pragma once
#include "MathLab/Vector3.h"
#include "CrossPlatform/SColorRGBA.h"

namespace CrossPlatform {
	using MathLab::Vector3f;
	using CrossPlatform::SColorRGBA;

	class Polygon3DPointer
	{
		int m_p0;
		int m_p1;
		int m_p2;
	};

	class Polygon3DVertex
	{
		Vector3f m_v0;
		Vector3f m_v1;
		Vector3f m_v2;
	};

	class Polygon3D
	{
	public:
		Polygon3D(SColorRGBA color, int numOfPtr, Polygon3DPointer* vertexPointerList, int numOfVertex, MathLab::Vector2f* vertexList);
		~Polygon3D();

	private:
		SColorRGBA m_color;
		int m_numOfPtr;
		Polygon3DPointer* m_vertexPointerList;
		int m_numOfVertex;
		Polygon3DVertex* m_vertexList;
	};
}