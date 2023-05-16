#include "IFileMeta.h"
#include "Engine/EngineManager.h"
#include "CrossPlatform/File/FolderMeta.h"
#include "yaml-cpp/yaml.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

#include <filesystem>
#include <fstream>

namespace CrossPlatform
{
	using namespace std;
	using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std::filesystem;

	void IFileMeta::Save(const XenonObject* /*data*/ /*= nullptr*/)
	{
        m_header.GenerateMetadata();
	}

	void IFileMeta::Delete()
	{
		const String& filePath = GetFileHeader().GetFilePath();
		String metaFilePath = filePath + ".metadata";
		path modelMetaFile(metaFilePath.CString());
		bool result = remove(modelMetaFile);
		assert(result == true);
	}

}