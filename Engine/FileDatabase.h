#pragma once
#include "CrossPlatform/File/IFileMeta.h"
#include <string>

#include "CrossPlatform/DataPair.h"
#include "CrossPlatform/FileTypeEnum.h"
#include <map>

namespace CrossPlatform
{
    class FolderMeta;
}

namespace XenonEngine
{
    class FileDatabase
    {
    public:
        FileDatabase() = default;

        void Initialize();
        void Shutdown();

        CrossPlatform::FileType GetFileType(const std::string& ext)const;
        const CrossPlatform::FolderMeta* GetRootFolder()const { assert(m_root != nullptr); return m_root; }
        const CrossPlatform::IFileMeta* GetFile(xg::Guid fileGuid);
        const Algorithm::String& GetExtension(CrossPlatform::FileType fileType);

        // Virtual Path
        CrossPlatform::FolderMeta* GetFolderByVirtualPath(const Algorithm::String& virtualPath) const;
        CrossPlatform::FolderMeta* GetFolderByRealPath(const Algorithm::String& realPath) const;
        CrossPlatform::FolderMeta* CreateFolderByVirtualPath(const Algorithm::String& virtualPath) const;
        CrossPlatform::FolderMeta* CreateFolderByRealPath(const Algorithm::String& realPath) const;
        Algorithm::String ConvertToVirtualPath(const Algorithm::String& virtualPath)const;
        Algorithm::String ConvertToRealPath(const Algorithm::String& realPath)const;

        // For Database
        const CrossPlatform::IFileMeta* AddFile(const Algorithm::String& realPath);
        // For Engine
        void LoadFile(const Algorithm::String& realPath);
        void SaveFile(const Algorithm::String& realPath);
    private:
        bool IsVirtualPath(const Algorithm::String& filePath)const;
        void RecursionFindFolder(CrossPlatform::FolderMeta& folder);
        void RecursionClearFolder(CrossPlatform::FolderMeta& folder);
        CrossPlatform::FolderMeta* m_root = nullptr;
        Algorithm::Vector<CrossPlatform::DataPair> m_typePair;
        std::map<xg::Guid, CrossPlatform::IFileMeta*> m_database;
    };
}