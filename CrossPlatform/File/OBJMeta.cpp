#include "ImageMeta.h"
#include "CrossPlatform/Image/Image.h"
#include <filesystem>
namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;

	void OBJMeta::Load()
	{
		bool result = ObjectLoader::Get().LoadObj(m_header.GetFilePath());
		assert(result == true);
	}

	void OBJMeta::Clear()
	{
	}

	void OBJMeta::Save()
	{
		IFileMeta::Save();
	}

	void OBJMeta::Delete()
	{
		delete m_gameobjectWorld;
		m_gameobjectWorld = nullptr;

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			path modelFile(filePath.CString());
			bool result = remove(modelFile);
			assert(result == true);
		}
		{
			String metaFilePath = filePath + ".metadata";
			path modelMetaFile(metaFilePath.CString());
			bool result = remove(modelMetaFile);
			assert(result == true);
		}
	}

}