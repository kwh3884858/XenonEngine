#include "FolderMeta.h"
#include <filesystem>
#include <fstream>

#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

namespace CrossPlatform
{
    using namespace std;
    using namespace std::filesystem;
    using namespace Algorithm;

	void FolderMeta::Load()
	{

	}

	void FolderMeta::Clear()
	{

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

		FolderMeta* folder = CreateFolder(folder.parent_path().string().c_str());
		String fileName(folder.filename().string().c_str());
		IFileMeta* file = folder->GetFile(fileName);

		xg::Guid guid = xg::newGuid();
		FolderMeta* folderMeta = new FolderMeta(FileHeader(FileType::FileTypeFolder, filePath, guid));
		return folderMeta;

		if (file)
		{
			GameObjectWorldMeta* metaFile = (GameObjectWorldMeta*)file;
			metaFile->SaveGameObjectWorld();
		}
		else
		{
			GameObjectWorldMeta* metaFile = (GameObjectWorldMeta*)AddFile(filePath);
			assert(metaFile != nullptr);
			GameObjectWorld* world = EngineManager::Get().GetWorldManager().GetCurrentWorld();
			world->SetWorldName(folder.stem().string().c_str());
			metaFile->SetGameObjectWorld(world);
			metaFile->SaveGameObjectWorld();
		}
	}

	void FolderMeta::Delete()
	{
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

	void FolderMeta::Registration()
	{
		RegisterFileFacotry(FileType::FileTypeFolder, Create);
		RegisterFileLoader(FileType::FileTypeFolder, Read);
	}


	IFileMeta* FolderMeta::Create(const Algorithm::String& filePath)
	{
		xg::Guid guid = xg::newGuid();
		FolderMeta* folderMeta = new FolderMeta(FileHeader(FileType::FileTypeFolder, filePath, guid));
		return folderMeta;
	}

	CrossPlatform::IFileMeta* FolderMeta::Read(const FileHeader& fileHeader)
	{
		FolderMeta* folderMeta = new FolderMeta(fileHeader);
		return folderMeta;
	}

}