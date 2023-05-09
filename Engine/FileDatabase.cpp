#pragma once
#include "FileDatabase.h"

#include "Algorithms/Vector.h"
#include "Algorithms/String.h"

#include "CrossPlatform/Database.h"
#include "CrossPlatform/File/Mesh3DMeta.h"
#include "CrossPlatform/DataPair.h"

#include <filesystem>
#include "yaml-cpp/yaml.h"

#include "CrossPlatform/File/FileMetaRegister.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"
#include "CrossPlatform/File/GameObjectWorldMeta.h"
#include "CrossPlatform/File/FolderMeta.h"
#include "CrossPlatform/File/MaterialMeta.h"
#include "CrossPlatform/File/ImageMeta.h"
#include "CrossPlatform/File/OBJMeta.h"
#include "CrossPlatform/File/Polygon3DMeta.h"

#include "Engine/EngineManager.h"
#include "Engine/GameObjectWorld.h"
//
//namespace CrossPlatform
//{
//	//DEFINE_FILE_TYPE(FileType::FileTypeFolder, CrossPlatform::FolderMeta)
//	//	DEFINE_FILE_TYPE(FileType::FileTypeMesh3D, CrossPlatform::Mesh3DMeta)
//	//	DEFINE_FILE_TYPE(FileType::FileTypePolygon, CrossPlatform::Polygon3DMeta)
//	//	DEFINE_FILE_TYPE(FileType::FileTypeMaterial, CrossPlatform::MaterialMeta)
//	//	DEFINE_FILE_TYPE(FileType::FileTypeWorld, CrossPlatform::GameObjectWorldMeta)
//	//	DEFINE_FILE_TYPE(FileType::FileTypeImage, CrossPlatform::ImageMeta)
// //       DEFINE_FILE_TYPE(FileType::FileTypeImage, CrossPlatform::OBJMeta)
//}

namespace XenonEngine
{
    using namespace std;
    using namespace std::filesystem;
    using namespace CrossPlatform;
    using namespace Algorithm;

    void FileDatabase::Initialize()
    {
		m_typePair.Add(DataPair(FileType::FileTypeFolder, ""));
		m_typePair.Add(DataPair(FileType::FileTypeMesh3D, ".xmesh3d"));
		m_typePair.Add(DataPair(FileType::FileTypePolygon, ".xpolygon"));
		m_typePair.Add(DataPair(FileType::FileTypeMaterial, ".xmaterial"));
		//m_typePair.Add(DataPair(CrossPlatform::FileTypeObjMaterial, ".mtl"));
		m_typePair.Add(DataPair(FileType::FileTypeWorld, ".world"));
		m_typePair.Add(DataPair(FileType::FileTypeImage, ".jpg"));
		m_typePair.Add(DataPair(FileType::FileTypeImage, ".png"));
		m_typePair.Add(DataPair(FileType::FileTypeImage, ".dds"));
		m_typePair.Add(DataPair(FileType::FileTypeObjFormatFile, ".obj"));

		//FolderMeta::Registration();
		static FileMetaRegister<IFileMeta, FolderMeta>folder(FileType::FileTypeFolder);
		static FileMetaRegister<IFileMeta, Mesh3DMeta>mesh3D(FileType::FileTypeMesh3D);
		static FileMetaRegister<IFileMeta, Polygon3DMeta>polygon3D(FileType::FileTypePolygon);
		static FileMetaRegister<IFileMeta, MaterialMeta>material(FileType::FileTypeMaterial);
		static FileMetaRegister<IFileMeta, GameObjectWorldMeta>world(FileType::FileTypeWorld);
		static FileMetaRegister<IFileMeta, ImageMeta>imageFactory(FileType::FileTypeImage);
		static FileMetaRegister<IFileMeta, OBJMeta>objFormat(FileType::FileTypeObjFormatFile);


        path projectRoot(CrossPlatform::Database::Get().engineConfig.m_projectPath.CString());
        projectRoot.make_preferred();
        if (!exists(projectRoot))
        {
            create_directory(projectRoot);
        }

		path projectDataRoot(projectRoot);
		projectDataRoot += path::preferred_separator;
		projectDataRoot = projectRoot.append("Data");
		if (!exists(projectDataRoot))
		{
			create_directory(projectDataRoot);
            FolderMeta* folderMeta = (FolderMeta*) CreateMetaFromFilePath(projectDataRoot.string().c_str());
            m_root = folderMeta;
		}
        else
        {
			FolderMeta* root = new FolderMeta( FileHeader(FileType::FileTypeFolder, projectDataRoot.string().c_str(), xg::Guid()) );
			m_root = root;
            //LoadFile(projectDataRoot.string().c_str());
        }

        m_root->Load();
    }

    void FileDatabase::Shutdown()
    {
		assert(m_root != nullptr);
		if (m_root)
		{
			m_root->Clear();
			delete m_root;
			m_root = nullptr;
		}
    }

	void FileDatabase::RefreshContent()
	{
		assert(m_root != nullptr);
		if (m_root)
		{
			m_root->Clear();
			m_root->Load();
		}
	}

	CrossPlatform::FileType FileDatabase::GetFileType(const std::string& ext) const
	{
		for (int i = 0; i < m_typePair.Count(); i++)
		{
			if (ext == m_typePair[i].m_fileSuffix.CString())
			{
				return m_typePair[i].m_fileType;
			}
		}
		return FileType::FileTypeNone;
	}

    const CrossPlatform::IFileMeta* FileDatabase::GetFile(const xg::Guid& fileGuid) const
    {
        assert(m_database.find(fileGuid) != m_database.end());
        return m_database.at(fileGuid);
    }

	CrossPlatform::IFileMeta* FileDatabase::GetFile(const xg::Guid& fileGuid)
	{
		return const_cast<CrossPlatform::IFileMeta*>(static_cast<const FileDatabase&>(*this).GetFile(fileGuid));
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

    CrossPlatform::FolderMeta* FileDatabase::GetOrCreateFolder(const Algorithm::String& inPath)
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

        FolderMeta* currentFolder = m_root;
        String tmpVirtualPath = decompositionPath[0];
        for (int i = 1; i < decompositionPath.Count(); i++)
        {
            FolderMeta* tmpFolder = (FolderMeta*)currentFolder->GetFile(decompositionPath[i]);
            tmpVirtualPath.Append(std::filesystem::path::preferred_separator);
            tmpVirtualPath.Append(decompositionPath[i]);
            if (tmpFolder == nullptr)
            {
                String realPath = ConvertToRealPath(tmpVirtualPath);
				tmpFolder = (FolderMeta*)CreateMetaFromFilePath(realPath);
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

    IFileMeta* FileDatabase::GenerateMetaFileForFile(const Algorithm::String& realPath)
    {
        path originalFile(realPath.CString());
        FileType fileType = GetFileType(originalFile.extension().string());
		if (fileType == FileType::FileTypeNone)
		{
			assert(true == false);
		}
		//IFileMeta* meta = nullptr;
		FolderMeta* folder = GetOrCreateFolder(originalFile.parent_path().string().c_str());
		if (folder->GetFile(originalFile.filename().string().c_str()) == nullptr)
		{
            IFileMeta* meta = CreateMetaFromFilePath(realPath);
			folder->AddIFile(meta);
			return meta;
		}
		return nullptr;
    }

	void FileDatabase::DeleteFile(const xg::Guid& fileGuid)
	{
		IFileMeta* file = GetFile(fileGuid);
		const String& virtualPath = file->GetFileHeader().GetVirtualPath();
		InternalDeleteFile(virtualPath);
	}

	void FileDatabase::DeleteFile(const Algorithm::String& inPath)
	{
		String filePath(inPath);
		if (IsVirtualPath(filePath))
		{
			filePath = ConvertToRealPath(filePath);
		}
		InternalDeleteFile(filePath);
	}

	void* FileDatabase::LoadFile(const Algorithm::String& realPath)
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

		return file->Instantiate();
		//return file;
    }

    void FileDatabase::SaveFile(const Algorithm::String& realPath)
    {
        String filePath(realPath);
        if (IsVirtualPath(filePath))
        {
            filePath = ConvertToRealPath(filePath);
        }
        path originalFile(filePath.CString());
		FolderMeta* folder = GetOrCreateFolder(originalFile.parent_path().string().c_str());
		String fileName(originalFile.filename().string().c_str());
		IFileMeta* file = folder->GetFile(fileName);
		if (!file)
		{
			file = CreateMetaFromFilePath(filePath);
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

	void FileDatabase::InternalDeleteFile(const Algorithm::String& filePath)
	{
		path originalFile(filePath.CString());
		FolderMeta* folderMeta = GetFolder(originalFile.parent_path().string().c_str());
		IFileMeta* fileMeta = folderMeta->GetFile(originalFile.filename().string().c_str());
		if (fileMeta)
		{
			// Database
			m_database.erase(fileMeta->GetFileHeader().GetGUID());
			// Folder
			folderMeta->RemoveFile(fileMeta);
			// Itself virtual Delete function
			fileMeta->Delete();
			// delete it
			delete fileMeta;

			fileMeta = nullptr;
		}
	}

	//void FileDatabase::RecursiveLoadFolder(FolderMeta& parentFolder)
 //   {
 //       directory_iterator dataRoot(parentFolder.GetFileHeader().GetFilePath().CString());
 //       for (auto& it : dataRoot)
 //       {
 //           if (it.path().extension() == ".metadata")
 //           {
 //               path relatedFile = it.path().string().substr(0, it.path().string().find_last_of('.'));
 //               if (!exists(relatedFile))
 //               {
 //                   remove(it.path());
 //                   continue;
 //               }

 //               YAML::Node config = YAML::LoadFile(it.path().generic_string());
 //               FileHeader header = config.as<FileHeader>();
 //               if (header.GetFileType() == FileType::FileTypeNone)
 //               {
 //                   remove(it.path());
 //                   continue;
 //               }
 //               if (header.GetGUID() == xg::Guid())
 //               {
 //                   remove(it.path());
 //                   continue;
 //               }

 //               header.SetFilePath(relatedFile.string().c_str())
 //               IFileMeta* file = nullptr;
	//			const FileType fileType = header.GetFileType();
	//			assert(fileType != FileType::FileTypeNone);

	//			file = CreateFileMetaFromHeader<fileType>(header);
	//			if (fileType == FileType::FileTypeFolder)
	//			{
	//				FolderMeta* folder = (FolderMeta*)file;
	//				RecursiveLoadFolder(*folder);
	//			}
	//			assert(file != nullptr);

 //               parentFolder.AddIFile(file);
	//			AddFileToDatabase(header.GetGUID(), file);
 //           }
 //       }
 //   }

   // void FileDatabase::RecursionClearFolder(FolderMeta& folder)
   // {
   //     for (int i = 0; i < folder.GetFileCount(); i++)
   //     {
   //         IFileMeta* file = folder.GetFile(i);
   //         FileType type = file->GetFileHeader().GetFileType();
			//if (type == FileType::FileTypeFolder)
			//{
			//	FolderMeta* fileFolder = (FolderMeta*)file;
			//	RecursionClearFolder(*fileFolder);
			//}
			//file->Clear();
			//delete file;
			//file = nullptr;
   //     }
   // }

	CrossPlatform::IFileMeta* FileDatabase::CreateMetaFromFilePath(const Algorithm::String& realFilePath)
	{
        path stdFilePath(realFilePath.CString());
        if (!exists(stdFilePath))
        {
            printf("Meta related file [ %s ] is not exist yet.", realFilePath.CString());
        }
        const FileType fileType = GetFileType(stdFilePath.extension().string());
		xg::Guid guid = xg::newGuid();
        FileHeader header(fileType, realFilePath, guid);
        IFileMeta* meta = FileMetaFactory<IFileMeta>::Instance().GetProduct(header);
		meta->IFileMeta::Save();
		AddFileToDatabase(meta->GetFileHeader().GetGUID(), meta);
		return meta;
	}

	//IFileMeta* FileDatabase::GenerateMetaFile(const Algorithm::String& filePath)
	//{
 //       path stdFilePath(filePath.CString());
	//	FileType fileType = GetFileType(stdFilePath.extension().string());
	//	IFileMeta* meta = IFileMeta::CreateNewFileMeta(fileType, filePath);
	//	meta->IFileMeta::Save();
	//	AddFileToDatabase(meta->GetFileHeader().GetGUID(), meta);
 //       return meta;
	//}

}