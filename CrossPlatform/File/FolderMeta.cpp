#include "FolderMeta.h"
#include <filesystem>
#include <fstream>

#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

#include "Engine/EngineManager.h"

namespace CrossPlatform
{
    using namespace std;
    using namespace std::filesystem;
    using namespace Algorithm;
	using namespace XenonEngine;

	void FolderMeta::Load()
	{
		directory_iterator dataRoot(GetFileHeader().GetFilePath().CString());
		for (auto& it : dataRoot)
		{
			if (it.path().extension() == ".metadata")
			{
				path relatedFile = it.path().string().substr(0, it.path().string().find_last_of('.'));
				if (!exists(relatedFile))
				{
					remove(it.path());
					continue;
				}

				YAML::Node config = YAML::LoadFile(it.path().generic_string());
				FileHeader header = config.as<FileHeader>();

				if (header.GetFileType() == FileType::FileTypeNone)
				{
					remove(it.path());
					continue;
				}
				if (header.GetGUID() == xg::Guid())
				{
					remove(it.path());
					continue;
				}

				header.SetFilePath(relatedFile.string().c_str());
				IFileMeta* file = nullptr;
				constexpr FileType fileType = header.GetFileType();
				assert(fileType != FileType::FileTypeNone);

				file = CreateFileMetaFromHeader<fileType>(header);
				if (fileType == FileType::FileTypeFolder)
				{
					FolderMeta* folder = (FolderMeta*)file;
					folder->Load();
				}
				assert(file != nullptr);

				AddIFile(file);
				EngineManager::Get().GetFileDatabase().AddFileToDatabase(header.GetGUID(), file);
			}
		}
	}

	void FolderMeta::Clear()
	{
		for (int i = 0; i < GetFileCount(); i++)
		{
			IFileMeta* file = GetFile(i);
			FileType type = file->GetFileHeader().GetFileType();
			if (type == FileType::FileTypeFolder)
			{
				FolderMeta* fileFolder = (FolderMeta*)file;
				fileFolder->Clear();
			}
			file->Clear();
			delete file;
			file = nullptr;
		}
	}

	void FolderMeta::Save()
	{
		IFileMeta::Save();

		const String& filePath = GetFileHeader().GetFilePath();
		path folder(filePath.CString());
		if (!exists(folder))
		{
			create_directory(folder);
		}
	}

	void FolderMeta::Delete()
	{
		IFileMeta::Delete();

		for (int i =0 ; i < m_content.Count(); i++)
		{
			m_content[i]->Delete();
			RemoveFile(m_content[i]);
		}
		path folderPath(GetFileHeader().GetFilePath().CString());
		remove(folderPath);
		assert(m_content.Count() == 0); 
	}

	CrossPlatform::IFileMeta* FolderMeta::GetFile(const Algorithm::String& fileName) const
    {
        for (int i = 0 ; i < m_content.Count(); i++)
        {
            if (m_content[i]->GetFileHeader().GetFileName() == fileName)
            {
                return m_content[i];
            }
        }
        return nullptr;
    }

    bool FolderMeta::CreateFolder() const
    {
        path folderPath(GetFileHeader().GetFilePath().CString());
        if (!exists(folderPath))
        {
            create_directory(folderPath);
        }
        return true;
    }

	//void FolderMeta::Registration()
	//{
	//	RegisterFileFacotry(FileType::FileTypeFolder, Create);
	//	RegisterFileLoader(FileType::FileTypeFolder, Read);
	//}


	//IFileMeta* FolderMeta::Create(const Algorithm::String& filePath)
	//{
	//	xg::Guid guid = xg::newGuid();
	//	FolderMeta* folderMeta = new FolderMeta(FileHeader(FileType::FileTypeFolder, filePath, guid));
	//	return folderMeta;
	//}

	//CrossPlatform::IFileMeta* FolderMeta::Read(const FileHeader& fileHeader)
	//{
	//	FolderMeta* folderMeta = new FolderMeta(fileHeader);
	//	return folderMeta;
	//}

}