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

	//class Polygon3DVertex
	//{
	//	Vector3f m_v0;
	//	Vector3f m_v1;
	//	Vector3f m_v2;
	//};

	class Polygon3D
	{
	public:
		Polygon3D(int numOfPtr, Polygon3DPointer* vertexPointerList, int numOfVertex, Polygon3DVertex* vertexList);
		Polygon3D(const Polygon3D& that);
		~Polygon3D();

	private:
		int m_numOfIndex = 0;
		int* m_vertexIndexList = nullptr;
		int m_numOfVertex = 0;
		Vector3f* m_vertexList = nullptr;
	};

	class Triangle
	{
	public:
		Triangle();
		~Triangle();

	private:

	};

	Triangle::Triangle()
	{
	}

	Triangle::~Triangle()
	{
	}
}