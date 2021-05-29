#include "Engine/FileManager/FileManager.h"
#include "CrossPlatform/Interface/IFileReader.h"

namespace XenonEnigne {

    using CrossPlatform::IFileReader;

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