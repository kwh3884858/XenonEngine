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

namespace XenonEngine
{
    using namespace std;
    using namespace std::filesystem;
    using namespace CrossPlatform;

    void FileDatabase::Initialize()
    {
        m_typePair.Add(DataPair(CrossPlatform::FileTypeModel, "obj"));
        m_typePair.Add(DataPair(CrossPlatform::FileTypeMaterial, "mtl"));
        m_typePair.Add(DataPair(CrossPlatform::FileTypeWorld, "world"));

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

        m_root = new FolderMeta(FileHeader(FileTypeFolder, m_projectData.string().c_str(), xg::Guid(m_projectData.string().c_str())));
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

    void FileDatabase::RecursionFindFolder(const FolderMeta& parentFolder)
    {
        directory_iterator dataRoot(parentFolder.GetFileHeader().GetFilePath().CString());
        for (auto& it : dataRoot)
        {
            if (it.path().extension() == "metadata")
            {
                path relatedFile = it.path().string().substr(0, 9);
                if (!exists(relatedFile))
                {
                    remove(it.path());
                    continue;
                }

                YAML::Node config = YAML::LoadFile(it.path().generic_string());
                FileHeader header = config["header"].as<FileHeader>();
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
                m_root->AddIFile(file);
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