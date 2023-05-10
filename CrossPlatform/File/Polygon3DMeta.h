#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace XenonEngine
{
	class ObjectImporter;
}

namespace CrossPlatform
{
	class Polygon3D;
	class Polygon3DMeta :public IFileMeta
	{
	public:
		friend class XenonEngine::ObjectImporter;

		Polygon3DMeta(const FileHeader& header) :IFileMeta(header) {}
		virtual ~Polygon3DMeta()override = default;

		//virtual void Add() override;

		virtual Polygon3D* Instantiate() override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save(const Polygon3D* polygon) override;

		// Delete data file from hard drive
		virtual void Delete() override;

		//const Polygon3D* const GetPolygon3D()const { return m_polygon; }
		//Polygon3D* const GetPolygon3D() { return m_polygon; }
	private:
		//Polygon3D* m_polygon = nullptr;
	};
}