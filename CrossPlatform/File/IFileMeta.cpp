#include "IFileMeta.h"
#include "Engine/EngineManager.h"
#include "CrossPlatform/File/FolderMeta.h"
#include <filesystem>
namespace CrossPlatform
{
    using namespace XenonEngine;
    using namespace Algorithm;
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
        return m_filePath.Substring(delimiterIndex +1, m_filePath.Count());
    }

}