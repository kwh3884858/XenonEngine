#include "Engine/FileManager/FileManager.h"

namespace XenonEnigne {
    void FileManager::SetFileReader(CrossPlatform::IFileReader*const fileReader)
    {
        m_fileReader = fileReader;
    }

    bool FileManager::Shutdown()
    {
        m_fileReader = nullptr;
        return true;
    }

    XenonFile* FileManager::ReadFile(const char* const fileName, char** file)
    {
        m_fileReader->ReadFile(fileName);
    }
}