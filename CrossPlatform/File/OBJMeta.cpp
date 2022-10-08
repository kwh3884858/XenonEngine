#include "OBJMeta.h"
#include "Engine/IO/ObjLoader.h"
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
		IFileMeta::Delete();

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
	}

}