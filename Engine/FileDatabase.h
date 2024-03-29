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

        void RefreshContent();

        CrossPlatform::FileType GetFileType(const std::string& ext)const;
        const CrossPlatform::FolderMeta* GetRootFolder()const { assert(m_root != nullptr); return m_root; }
        const CrossPlatform::IFileMeta* GetFile(const xg::Guid& fileGuid) const;
        CrossPlatform::IFileMeta* GetFile(const xg::Guid& fileGuid);
        const Algorithm::String& GetExtension(CrossPlatform::FileType fileType);

        // Virtual Path
        CrossPlatform::FolderMeta* GetFolder(const Algorithm::String& virtualPath) const;
        CrossPlatform::FolderMeta* GetOrCreateFolder(const Algorithm::String& virtualPath);
        Algorithm::String ConvertToVirtualPath(const Algorithm::String& virtualPath)const;
        Algorithm::String ConvertToRealPath(const Algorithm::String& realPath)const;

        // For Database
        CrossPlatform::IFileMeta* GenerateMetaFileForFile(const Algorithm::String& realPath);
		// Delete file from database, folder and itself virtual function "Delete".
		void DeleteFile(const xg::Guid& fileGuid);
		void DeleteFile(const Algorithm::String& path);

        // For Engine
		void* LoadFile(const Algorithm::String& realPath);
        void SaveFile(const Algorithm::String& realPath);

		Algorithm::String ProcessFileNameToFullPath(const Algorithm::String& fileName, const Algorithm::String& currentFolder);

        void AddFileToDatabase(const xg::Guid& guid, CrossPlatform::IFileMeta* file) { m_database[guid] = file; }
        void RemoveFileFromDatabse(const xg::Guid& guid) { m_database.erase(guid); }

    private:
		bool IsVirtualPath(const Algorithm::String& filePath)const;
		bool IsRealPath(const Algorithm::String& filePath)const;

		void InternalDeleteFile(const Algorithm::String& filePath);
        
		IFileMeta* CreateMetaFromFilePath(const Algorithm::String& realFilePath);

		//void RecursiveLoadFolder(CrossPlatform::FolderMeta& folder);
        //void RecursionClearFolder(CrossPlatform::FolderMeta& folder);

        //IFileMeta* GenerateMetaFile(const Algorithm::String& filePaht);

        CrossPlatform::FolderMeta* m_root = nullptr;
        Algorithm::Vector<CrossPlatform::DataPair> m_typePair;
        std::map<xg::Guid, CrossPlatform::IFileMeta*> m_database;
		std::map<CrossPlatform::FileType, type_info> m_fileTypePair;
	};
}