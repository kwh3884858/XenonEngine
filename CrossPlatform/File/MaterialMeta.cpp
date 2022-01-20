#include <filesystem>
#include "MaterialMeta.h"
#include "CrossPlatform/Material/Material.h"

namespace CrossPlatform
{
	using namespace Algorithm;

	MaterialMeta::~MaterialMeta()
	{
		delete m_material;
		m_material = nullptr;
	}

	void MaterialMeta::Delete()
	{
		delete m_material;
		m_material = nullptr;

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			std::filesystem::path meterialFile(filePath.CString());
			bool result = remove(meterialFile);
			assert(result == true);
		}
		{
			String metaFilePath = filePath + ".metadata";
			std::filesystem::path modelMetaFile(metaFilePath.CString());
			bool result = remove(modelMetaFile);
			assert(result == true);
		}
	}

}