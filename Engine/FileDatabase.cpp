#pragma once
#include "FileDatabase.h"
#include "Algorithms/Vector.h"
#include "Algorithms/String.h"
#include "CrossPlatform/Database.h"
#include "CrossPlatform/IFile.h"
#include "CrossPlatform/DataPair.h"
#include <filesystem>
#include "yaml-cpp/yaml.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

namespace XenonEngine
{
    using namespace std;
    using namespace std::filesystem;
    using namespace CrossPlatform;

    void FileDatabase::Initialize()
    {
        m_typePair.Add(DataPair(CrossPlatform::FileTypeModel, "obj"));
        m_typePair.Add(DataPair(CrossPlatform::FileTypeMaterial, "mtl"));

        path Project(CrossPlatform::Database::Get().engineConfig.m_projectPath.CString());
        if (!exists(Project))
        {
            create_directory(Project);
        }
        path m_projectData = Project.append("Data");
        if (!exists(m_projectData))
        {
            create_directory(m_projectData);
        }

        m_root = new Folder(FileHeader(FileTypeFolder, "Root", xg::Guid()));
        directory_iterator dataRoot(m_projectData);
        for (auto& it : dataRoot)
        {
            if (it.is_directory())
            {
            }
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
                IFile* file = nullptr;
                if (header.GetFileType() == FileType::FileTypeFolder)
                {
                    file = new Folder(header);
                }
                if (header.GetFileType() == FileType::FileTypeMaterial)
                {
                    file = new Material(header);
                }
                if (header.GetFileType() == FileType::FileTypeModel)
                {
                    file = new Model(header);
                }
                m_root->AddIFile(file);
            }
        }
    }

    void FileDatabase::Shutdown()
    {
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

}