#include "OBJMeta.h"
#include "Engine/IO/ObjectImporter.h"
#include <filesystem>
namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;

	void OBJMeta::OnImport()
	{
		bool result = ObjectImporter::Get().ImportObj(m_header.GetFilePath());
		assert(result == true);
	}

	void OBJMeta::Save(const XenonObject* /*data*/ /*= nullptr*/)
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