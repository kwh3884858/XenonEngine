#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace XenonEngine
{
	class ObjectLoader;
}

namespace CrossPlatform
{
	class Polygon3D;
	class Polygon3DMeta :public IFileMeta
	{
	public:
		friend class XenonEngine::ObjectLoader;

		Polygon3DMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypePolygon); }
		virtual ~Polygon3DMeta()override = default;

		// Load into memory
		virtual void Load() override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save() override;

		// Delete data file from hard drive
		virtual void Delete() override;

		const Polygon3D* const GetPolygon3D()const { return m_polygon; }
		//Polygon3D* const GetPolygon3D() { return m_polygon; }
	private:
		Polygon3D* m_polygon = nullptr;
	};
}