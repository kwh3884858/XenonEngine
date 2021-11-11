#include "EditorDatabase.h"
#include "CrossPlatform/File/FolderMeta.h"
#include <filesystem>

namespace XenonEngine
{
    using namespace Algorithm;
    using namespace CrossPlatform;

    CrossPlatform::FolderMeta* EditorDatabase::GetFolder(const Algorithm::String& virtualPath) const
    {
        int rootIndex = m_rootFolder->GetFileHeader().GetFilePath().Count();
        Vector<String> decompositionPath = virtualPath.Split(std::filesystem::path::preferred_separator);
        if (decompositionPath.Count() < 1)
        {
            throw "Path Error";
            return nullptr;
        }
        //Only for Windows
        if (decompositionPath.Count() == 1)
        {
            assert(decompositionPath[0] == FileHeader::Root_Drive);
        }
        FolderMeta* currentFolder =(FolderMeta*) m_rootFolder;
        for (int i = 1; i < decompositionPath.Count(); i++)
        {
            currentFolder =(FolderMeta*) currentFolder->GetFile(decompositionPath[i]);
            if (currentFolder == nullptr)
            {
                return nullptr;
            }
            if (currentFolder->GetFileHeader().GetFileType() != FileType::FileTypeFolder)
            {
                return nullptr;
            }
        }
        return currentFolder;
    }

    bool EditorDatabase::CreateFolder(const Algorithm::String& virtualPath) const
    {
        Vector<String> decompositionPath = virtualPath.Split(std::filesystem::path::preferred_separator);
        if (decompositionPath.Count() < 1)
        {
            throw "Path Error";
            return nullptr;
        }
        //Only for Windows
        if (decompositionPath.Count() == 1)
        {
            assert(decompositionPath[0] == FileHeader::Root_Drive);
        }
        FolderMeta* currentFolder = (FolderMeta*)m_rootFolder;
        String tmpVirtualPath = decompositionPath[0];
        for (int i = 1; i < decompositionPath.Count(); i++)
        {
            FolderMeta* tmpFolder = (FolderMeta*)currentFolder->GetFile(decompositionPath[i]);
            tmpVirtualPath.Append(std::filesystem::path::preferred_separator);
            tmpVirtualPath.Append(decompositionPath[i]);
            if (tmpFolder == nullptr)
            {
                String realPath = m_rootFolder->GetFileHeader().GetFilePath() + tmpVirtualPath.Substring(FileHeader::Root_Drive.Count(), tmpVirtualPath.Count());
                
                xg::Guid guid = xg::newGuid();
                tmpFolder = new FolderMeta(FileHeader(FileType::FileTypeFolder, realPath, guid));
                tmpFolder->CreateFolder();
                currentFolder->AddIFile(tmpFolder);
            }
            if (tmpFolder->GetFileHeader().GetFileType() != FileType::FileTypeFolder)
            {
                throw "Same Name File";
                return false;
            }
            currentFolder = tmpFolder;
        }
    }

    Algorithm::String EditorDatabase::ConvertVirtualPath(const Algorithm::String& virtualPath)
    {
        return  m_rootFolder->GetFileHeader().GetFilePath() + virtualPath.Substring(FileHeader::Root_Drive.Count(), virtualPath.Count());
    }

}