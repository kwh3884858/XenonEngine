#include "FileHeader.h"
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
        int separatorIndex = m_filePath.LastIndexOf(std::filesystem::path::preferred_separator);
        assert(separatorIndex >= 0);
        return m_filePath.Substring(separatorIndex +1, m_filePath.Count());
    }

	Algorithm::String FileHeader::GetFileNameWithoutSuffix() const
	{
        Algorithm::String filename = GetFileName();
        int dotIndex = filename.LastIndexOf('.');
        assert(dotIndex >= 0);
		return filename.Substring(0, dotIndex);
	}

	void FileHeader::GenerateMetadata() const
    {
        String metaPath = GetFilePath() + ".metadata";
        ofstream outputStream(metaPath.CString());
        YAML::Emitter out(outputStream);
        out << YAML::Node(*this);
        outputStream.close();
    }

}