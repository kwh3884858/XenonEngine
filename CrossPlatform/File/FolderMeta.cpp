#include "FolderMeta.h"
#include <filesystem>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

namespace CrossPlatform
{
    using namespace std;
    using namespace std::filesystem;
    using namespace Algorithm;

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
        if (exists(folderPath))
        {
            return false;
        }
        create_directory(folderPath);

        String metaPath = GetFileHeader().GetFilePath() + ".metadata";
        ofstream outputStream(metaPath.CString());
        YAML::Emitter out(outputStream);
        out << YAML::Node(GetFileHeader());
        outputStream.close();
        return true;
    }

}