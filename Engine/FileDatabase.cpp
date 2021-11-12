#pragma once
#include "FileDatabase.h"
#include "Algorithms/Vector.h"
#include "Algorithms/String.h"
#include "CrossPlatform/Database.h"
#include "CrossPlatform/File/ModelMeta.h"
#include "CrossPlatform/DataPair.h"
#include <filesystem>
#include "yaml-cpp/yaml.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"
#include "CrossPlatform/File/WorldMeta.h"
#include "CrossPlatform/File/FolderMeta.h"

namespace XenonEngine
{
    using namespace std;
    using namespace std::filesystem;
    using namespace CrossPlatform;
    using namespace Algorithm;

    void FileDatabase::Initialize()
    {
        m_typePair.Add(DataPair(CrossPlatform::FileTypeModel, ".obj"));
        m_typePair.Add(DataPair(CrossPlatform::FileTypeMaterial, ".mtl"));
        m_typePair.Add(DataPair(CrossPlatform::FileTypeWorld, ".world"));

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

    const CrossPlatform::IFileMeta* FileDatabase::GetFile(xg::Guid fileGuid)
    {
        assert(m_database.find(fileGuid) != m_database.end());
        return m_database[fileGuid];
    }

    CrossPlatform::FolderMeta* FileDatabase::GetFolderByVirtualPath(const Algorithm::String& virtualPath) const
    {
        int rootIndex = m_root->GetFileHeader().GetFilePath().Count();
        Vector<String> decompositionPath = virtualPath.Split(std::filesystem::path::preferred_separator);
        if (decompositionPath.Count() < 1)
        {
            throw "Path Error";
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

    CrossPlatform::FolderMeta* FileDatabase::GetFolderByRealPath(const Algorithm::String& realPath) const
    {
        String virtualPath = ConvertToVirtualPath(realPath);
        return GetFolderByVirtualPath(virtualPath);
    }

    CrossPlatform::FolderMeta* FileDatabase::CreateFolderByVirtualPath(const Algorithm::String& virtualPath) const
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

                xg::Guid guid = xg::newGuid();
                tmpFolder = new FolderMeta(FileHeader(FileType::FileTypeFolder, realPath, guid));
                tmpFolder->GetFileHeader().GenerateMetadata();
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
        return currentFolder;
    }

    CrossPlatform::FolderMeta* FileDatabase::CreateFolderByRealPath(const Algorithm::String& realPath) const
    {
        return CreateFolderByVirtualPath(ConvertToVirtualPath(realPath));
    }

    Algorithm::String FileDatabase::ConvertToRealPath(const Algorithm::String& virtualPath)const
    {
        return m_root->GetFileHeader().GetFilePath() + virtualPath.Substring(FileHeader::Root_Drive.Count(), virtualPath.Count());
    }
    
    Algorithm::String FileDatabase::ConvertToVirtualPath(const Algorithm::String& realPath) const
    {
        return FileHeader::Root_Drive + realPath.Substring(m_root->GetFileHeader().GetFilePath().Count(), realPath.Count());
    }

    void FileDatabase::AddFile(const Algorithm::String& filePath)
    {
        path originalFile(filePath.CString());
        FileType fileType = GetFileType(originalFile.extension().string());
        switch (fileType)
        {
        case CrossPlatform::None:
            throw "Error Type";
            break;
        case CrossPlatform::FileTypeFolder:
            throw "How?";
            break;
        case CrossPlatform::FileTypeMaterial:
            break;
        case CrossPlatform::FileTypeModel:
        {
            FolderMeta* folder = CreateFolderByRealPath(originalFile.parent_path().string().c_str());
            if (folder->GetFile(originalFile.filename().string().c_str()) == nullptr)
            {
                xg::Guid guid = xg::newGuid();
                ModelMeta* modeMeta = new ModelMeta(FileHeader(fileType, originalFile.string().c_str(), guid));
                modeMeta->GetFileHeader().GenerateMetadata();
                folder->AddIFile(modeMeta);
            }
        }
            break;
        case CrossPlatform::FileTypeWorld:
            break;
        default:
            throw "Type Undefined";
            break;
        }
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
                if (header.GetFileType() == FileType::FileTypeFolder)
                {
                    FolderMeta* folder= new FolderMeta(header);
                    RecursionFindFolder(*folder);
                    file = (IFileMeta*)folder;
                }
                if (header.GetFileType() == FileType::FileTypeMaterial)
                {
                    file = new MaterialMeta(header);
                }
                if (header.GetFileType() == FileType::FileTypeModel)
                {
                    file = new ModelMeta(header);
                }
                if (header.GetFileType() == FileType::FileTypeWorld)
                {
                    file = new WorldMeta(header);
                }
                assert(file != nullptr);
                parentFolder.AddIFile(file);
                m_database[header.GetGUID()] = file;
            }
        }
    }

    void FileDatabase::RecursionClearFolder(FolderMeta& folder)
    {
        for (int i = 0; i < folder.GetFileCount(); i++)
        {
            IFileMeta* file = folder.GetFile(i);
            FileType type = file->GetFileHeader().GetFileType();
            switch (type)
            {
            case CrossPlatform::None:
                throw "File Type Is Undefined";
                break;
            case CrossPlatform::FileTypeFolder:
            {
                FolderMeta* folder = (FolderMeta*)file;
                RecursionClearFolder(*folder);
                delete folder;
            }
                break;
            default:
            {
                delete file;
                throw "File Type Is Undefined";
            }
                break;
            } 
        }
    }

}