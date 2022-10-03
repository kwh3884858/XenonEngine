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
    const Algorithm::String FileHeader::Root_Drive = "X:";

    Algorithm::String FileHeader::GetVirtualPath() const
    {
         const FolderMeta* rootFolder = EngineManager::Get().GetFileDatabase().GetRootFolder();
         String virtualPath = m_filePath.Substring(rootFolder->GetFileHeader().GetFilePath().Count(), m_filePath.Count());
         return Root_Drive + std::filesystem::path::preferred_separator + virtualPath;
    }

    Algorithm::String FileHeader::GetFileName() const
    {
        int delimiterIndex =m_filePath.LastIndexOf(std::filesystem::path::preferred_separator);
        assert(delimiterIndex >= 0);
        return m_filePath.Substring(delimiterIndex +1, m_filePath.Count());
    }

    void FileHeader::GenerateMetadata() const
    {
        String metaPath = GetFilePath() + ".metadata";
        ofstream outputStream(metaPath.CString());
        YAML::Emitter out(outputStream);
        out << YAML::Node(*this);
        outputStream.close();
    }

	void IFileMeta::Save()
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