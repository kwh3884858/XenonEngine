#pragma once
#include "CrossPlatform/File/IFileMeta.h"
#include <string>

#include "CrossPlatform/DataPair.h"
#include "CrossPlatform/FileTypeEnum.h"
#include <map>
#include <filesystem>

namespace CrossPlatform
{
    class FolderMeta;
}

namespace XenonEngine
{
	using namespace CrossPlatform;

    class FileDatabase
    {
    public:
        FileDatabase() = default;

        void Initialize();
        void Shutdown();

        CrossPlatform::FileType GetFileType(const std::string& ext)const;
        const CrossPlatform::FolderMeta* GetRootFolder()const { assert(m_root != nullptr); return m_root; }
        const CrossPlatform::IFileMeta* GetFile(const xg::Guid& fileGuid) const;
        const Algorithm::String& GetExtension(CrossPlatform::FileType fileType);

        // Virtual Path
        CrossPlatform::FolderMeta* GetFolder(const Algorithm::String& virtualPath) const;
        CrossPlatform::FolderMeta* CreateFolder(const Algorithm::String& virtualPath);
        Algorithm::String ConvertToVirtualPath(const Algorithm::String& virtualPath)const;
        Algorithm::String ConvertToRealPath(const Algorithm::String& realPath)const;

        // For Database
        CrossPlatform::IFileMeta* GenerateMetaFileForFile(const Algorithm::String& realPath);
		void DeleteFile(const Algorithm::String& path);

        // For Engine
		CrossPlatform::IFileMeta* LoadFile(const Algorithm::String& realPath);
        void SaveFile(const Algorithm::String& realPath);

		Algorithm::String ProcessFileName(const Algorithm::String& fileName, const Algorithm::String& currentFolder);
    private:
		bool IsVirtualPath(const Algorithm::String& filePath)const;
		bool IsRealPath(const Algorithm::String& filePath)const;
        void RecursiveLoadFolder(CrossPlatform::FolderMeta& folder);
        void RecursionClearFolder(CrossPlatform::FolderMeta& folder);

        IFileMeta* GenerateMetaFile(const Algorithm::String& filePaht);
		void AddFileToDatabase(const xg::Guid& guid, CrossPlatform::IFileMeta* file){ m_database[guid] = file; }

        CrossPlatform::FolderMeta* m_root = nullptr;
        Algorithm::Vector<CrossPlatform::DataPair> m_typePair;
        std::map<xg::Guid, CrossPlatform::IFileMeta*> m_database;
		std::map<CrossPlatform::FileType, type_info> m_fileTypePair;
	};
}