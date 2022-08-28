#pragma once
#include "FileDatabase.h"

#include "Algorithms/Vector.h"
#include "Algorithms/String.h"

#include "CrossPlatform/Database.h"
#include "CrossPlatform/File/Mesh3DMeta.h"
#include "CrossPlatform/DataPair.h"

#include <filesystem>
#include "yaml-cpp/yaml.h"

#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"
#include "CrossPlatform/File/WorldMeta.h"
#include "CrossPlatform/File/FolderMeta.h"
#include "CrossPlatform/File/MaterialMeta.h"
#include "CrossPlatform/File/ImageMeta.h"
#include "CrossPlatform/File/Polygon3DMeta.h"

#include "Engine/EngineManager.h"
#include "Engine/GameObjectWorld.h"

namespace XenonEngine
{
    using namespace std;
    using namespace std::filesystem;
    using namespace CrossPlatform;
    using namespace Algorithm;

    void FileDatabase::Initialize()
    {
		m_typePair.Add(DataPair(CrossPlatform::FileTypeFolder, ""));
		m_typePair.Add(DataPair(CrossPlatform::FileTypeMesh3D, ".xmesh3d"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypePolygon, ".obj"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypePolygon, ".fbx"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypeMaterial, ".xmaterial"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypeObjMaterial, ".mtl"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypeWorld, ".world"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypeImage, ".jpg"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypeImage, ".png"));
		m_typePair.Add(DataPair(CrossPlatform::FileTypeImage, ".dds"));

		FolderMeta::Initialization();


        path Project(CrossPlatform::Database::Get().engineConfig.m_projectPath.CString());
        Project.make_preferred();
        if (!exists(Project))
        {
            create_directory(Project);
        }
        path m_projectData = Project.append("Data");
        m_projectData.make_preferred();
        if (!exists(m_projectData))
        {
            create_directory(m_projectData);
        }
        xg::Guid guid = xg::newGuid();
        m_root = new FolderMeta(FileHeader(FileTypeFolder, m_projectData.string().c_str(), guid));
        RecursionFindFolder(*m_root);
    }

    void FileDatabase::Shutdown()
    {
        RecursionClearFolder(*m_root);
        delete m_root;
        m_root = nullptr;
    }

    FileType FileDatabase::GetFileType(const std::string& ext) const
    {
        for (int i = 0; i < m_typePair.Count(); i++)
        {
            if (ext == m_typePair[i].m_fileSuffix.CString())
            {
                return m_typePair[i].m_fileType;
            }
        }
        return FileType::None;
    }

    const CrossPlatform::IFileMeta* FileDatabase::GetFile(const xg::Guid& fileGuid) const
    {
        assert(m_database.find(fileGuid) != m_database.end());
        return m_database.at(fileGuid);
    }

    const Algorithm::String& FileDatabase::GetExtension(CrossPlatform::FileType fileType)
    {
        for (int i = 0; i < m_typePair.Count(); i++)
        {
            if (m_typePair[i].m_fileType == fileType)
            {
                return m_typePair[i].m_fileSuffix;
            }
        }
        assert(true == false);
		return Algorithm::String::INVALID_STRING;
    }

    CrossPlatform::FolderMeta* FileDatabase::GetFolder(const Algorithm::String& inPath) const
    {
		String virtualPath(inPath);
		if (!IsVirtualPath(virtualPath))
		{
			virtualPath = ConvertToVirtualPath(virtualPath);
		}
        //int rootIndex = m_root->GetFileHeader().GetFilePath().Count();
        Vector<String> decompositionPath = virtualPath.Split(std::filesystem::path::preferred_separator);
        if (decompositionPath.Count() < 1)
        {
            assert(true == false);

            return nullptr;
        }
        //Only for Windows

        if (decompositionPath[0] != FileHeader::Root_Drive)
        {
            return nullptr;
        }

        FolderMeta* currentFolder = (FolderMeta*)m_root;
        for (int i = 1; i < decompositionPath.Count(); i++)
        {
            currentFolder = (FolderMeta*)currentFolder->GetFile(decompositionPath[i]);
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

    //CrossPlatform::FolderMeta* FileDatabase::GetFolderByRealPath(const Algorithm::String& realPath) const
    //{
    //    String virtualPath = ConvertToVirtualPath(realPath);
    //    return GetFolderByVirtualPath(virtualPath);
    //}

    CrossPlatform::FolderMeta* FileDatabase::CreateFolder(const Algorithm::String& inPath)
    {
		String virtualPath(inPath);
		if (!IsVirtualPath(virtualPath))
		{
			virtualPath = ConvertToVirtualPath(virtualPath);
		}
        Vector<String> decompositionPath = virtualPath.Split(std::filesystem::path::preferred_separator);
        if (decompositionPath.Count() < 1)
        {
            assert(true == false);

            return nullptr;
        }
        //Only for Windows
        if (decompositionPath.Count() == 1)
        {
            assert(decompositionPath[0] == FileHeader::Root_Drive);
        }
        FolderMeta* currentFolder = (FolderMeta*)m_root;
        String tmpVirtualPath = decompositionPath[0];
        for (int i = 1; i < decompositionPath.Count(); i++)
        {
            FolderMeta* tmpFolder = (FolderMeta*)currentFolder->GetFile(decompositionPath[i]);
            tmpVirtualPath.Append(std::filesystem::path::preferred_separator);
            tmpVirtualPath.Append(decompositionPath[i]);
            if (tmpFolder == nullptr)
            {
                String realPath = m_root->GetFileHeader().GetFilePath() + tmpVirtualPath.Substring(FileHeader::Root_Drive.Count(), tmpVirtualPath.Count());

				tmpFolder = (FolderMeta*)IFileMeta::CreateNewFileMeta(FileType::FileTypeFolder, realPath);
                tmpFolder->GetFileHeader().GenerateMetadata();
                tmpFolder->CreateFolder();
                currentFolder->AddIFile(tmpFolder);
				AddFileToDatabase(tmpFolder->GetFileHeader().GetGUID(), tmpFolder);
            }
            if (tmpFolder->GetFileHeader().GetFileType() != FileType::FileTypeFolder)
            {
                assert(true == false);

                return nullptr;
            }
            currentFolder = tmpFolder;
        }
        return currentFolder;
    }

    Algorithm::String FileDatabase::ConvertToRealPath(const Algorithm::String& virtualPath)const
    {
        return m_root->GetFileHeader().GetFilePath() + virtualPath.Substring(FileHeader::Root_Drive.Count(), virtualPath.Count());
    }
    
    Algorithm::String FileDatabase::ConvertToVirtualPath(const Algorithm::String& realPath) const
    {
        return FileHeader::Root_Drive + realPath.Substring(m_root->GetFileHeader().GetFilePath().Count(), realPath.Count());
    }

    IFileMeta* FileDatabase::AddFile(const Algorithm::String& realPath)
    {
        path originalFile(realPath.CString());
        FileType fileType = GetFileType(originalFile.extension().string());
		if (fileType == FileType::None || FileType::FileTypeFolder)
		{
			assert(true == false);
		}
		IFileMeta* meta = nullptr;
		FolderMeta* folder = CreateFolder(originalFile.parent_path().string().c_str());
		if (folder->GetFile(originalFile.filename().string().c_str()) == nullptr)
		{
			meta = IFileMeta::CreateNewFileMeta(fileType, originalFile.string().c_str());
			meta->GetFileHeader().GenerateMetadata();
			folder->AddIFile(meta);
			AddFileToDatabase(meta->GetFileHeader().GetGUID(), meta);
		}
		return meta;
    }

	void FileDatabase::DeleteFile(const Algorithm::String& inPath)
	{
		String filePath(inPath);
		if (IsVirtualPath(filePath))
		{
			filePath = ConvertToRealPath(filePath);
		}
		path originalFile(filePath.CString());
		FolderMeta* folderMeta = GetFolder(originalFile.parent_path().string().c_str());
		IFileMeta* fileMeta = folderMeta->GetFile(originalFile.filename().string().c_str());
		if (fileMeta)
		{
			folderMeta->RemoveFile(fileMeta);
			FileType fileType = GetFileType(originalFile.extension().string());
			fileMeta->Delete();
			delete fileMeta;
			fileMeta = nullptr;
			
		}
	}

	IFileMeta* FileDatabase::LoadFile(const Algorithm::String& realPath)
    {
        String filePath(realPath);
        if (IsVirtualPath(filePath))
        {
            filePath = ConvertToRealPath(filePath);
        }
        path originalFile(filePath.CString());
		FolderMeta* folder = GetFolder(originalFile.parent_path().string().c_str());
		if (!folder)
		{
			assert(true == false);
			return nullptr;
		}
		IFileMeta* file = folder->GetFile(originalFile.filename().string().c_str());
		if (!file)
		{
			assert(true == false);
			return nullptr;
		}

		file->Load();
		return file;
    }

    void FileDatabase::SaveFile(const Algorithm::String& realPath)
    {
        String filePath(realPath);
        if (IsVirtualPath(filePath))
        {
            filePath = ConvertToRealPath(filePath);
        }
        path originalFile(filePath.CString());
		FolderMeta* folder = CreateFolder(originalFile.parent_path().string().c_str());
		String fileName(originalFile.filename().string().c_str());
		IFileMeta* file = folder->GetFile(fileName);
		if (!file)
		{
			file = AddFile(filePath);
		}
		file->Save();
    }

	Algorithm::String FileDatabase::ProcessFileName(const Algorithm::String& fileName, const Algorithm::String& currentFolder)
	{
		if (IsVirtualPath(fileName))
		{
			return fileName;
		}
		if (IsRealPath(fileName))
		{
			return fileName;
		}
		// "./fileName.txt" "/fileName.txt" "fileName.txt"
		if ((fileName[0] == '.' && fileName[1] == filesystem::path::preferred_separator) ||
			(fileName[0] == filesystem::path::preferred_separator) ||
			fileName.Find(filesystem::path::preferred_separator) == false)
		{
			return currentFolder + fileName;
		}
		assert(true == false);
		return String("Filename is not valid");
	}

	bool FileDatabase::IsVirtualPath(const Algorithm::String& filePath) const
    {
        return filePath[0] == 'X';
    }

	bool FileDatabase::IsRealPath(const Algorithm::String& filePath) const
	{
		return filePath.Find(m_root->GetFileHeader().GetFilePath());
	}

	void FileDatabase::RecursionFindFolder(FolderMeta& parentFolder)
    {
        directory_iterator dataRoot(parentFolder.GetFileHeader().GetFilePath().CString());
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
                if (header.GetFileType() == FileType::None)
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
				FileType fileType = header.GetFileType();
				assert(fileType != FileType::None);

				file = IFileMeta::CreateNewFileMeta(fileType, header);
				if (fileType == FileType::FileTypeFolder)
				{
					FolderMeta* folder = (FolderMeta*)file;
					RecursionFindFolder(*folder);
				}
				assert(file != nullptr);

                parentFolder.AddIFile(file);
				AddFileToDatabase(header.GetGUID(), file);
            }
        }
    }

    void FileDatabase::RecursionClearFolder(FolderMeta& folder)
    {
        for (int i = 0; i < folder.GetFileCount(); i++)
        {
            IFileMeta* file = folder.GetFile(i);
            FileType type = file->GetFileHeader().GetFileType();
			if (type == FileType::FileTypeFolder)
			{
				FolderMeta* fileFolder = (FolderMeta*)file;
				RecursionClearFolder(*fileFolder);
			}
			file->Delete();
			delete file;
			file = nullptr;
        }
    }

	//void FileDatabase::DeleteFileMeta(CrossPlatform::IFileMeta* fileMeta)
	//{
	//	fileMeta->Delete();
	//	delete fileMeta;
	//	fileMeta = nullptr;
	//}
}