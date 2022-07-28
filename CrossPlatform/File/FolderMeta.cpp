#include "FolderMeta.h"
#include <filesystem>

#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

namespace CrossPlatform
{
    using namespace std;
    using namespace std::filesystem;
    using namespace Algorithm;

	void FolderMeta::Save()
	{
		GetFileHeader().GetFilePath();
		FolderMeta* folder = CreateFolder(originalFile.parent_path().string().c_str());
		String fileName(originalFile.filename().string().c_str());
		IFileMeta* file = folder->GetFile(fileName);
		if (file)
		{
			WorldMeta* metaFile = (WorldMeta*)file;
			metaFile->SaveGameObjectWorld();
		}
		else
		{
			WorldMeta* metaFile = (WorldMeta*)AddFile(filePath);
			assert(metaFile != nullptr);
			GameObjectWorld* world = EngineManager::Get().GetWorldManager().GetCurrentWorld();
			world->SetWorldName(originalFile.stem().string().c_str());
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

	void FolderMeta::Initialization()
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