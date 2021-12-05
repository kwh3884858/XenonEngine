#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace CrossPlatform
{
	class Material;
	class MaterialMeta :public IFileMeta
	{
	public:
		MaterialMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeMaterial); }
		virtual ~MaterialMeta()override;
		virtual void Delete()override;
	private:
		Material* m_material;
	};
}