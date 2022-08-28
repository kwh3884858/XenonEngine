#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace CrossPlatform
{
	class Polygon3D;
	class Polygon3DMeta :public IFileMeta
	{
	public:
		Polygon3DMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypePolygon); }
		virtual ~Polygon3DMeta()override;
		virtual void Delete()override;

		const Polygon3D* const GetPolygon3D()const { return m_polygon; }
		//Polygon3D* const GetPolygon3D() { return m_polygon; }
	private:
		Polygon3D* m_polygon = nullptr;
	};
}