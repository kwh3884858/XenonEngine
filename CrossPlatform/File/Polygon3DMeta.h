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
	private:
		Polygon3D* m_polygon;
	};
}