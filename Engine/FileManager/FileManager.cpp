#include "Engine/FileManager/FileManager.h"
#include "CrossPlatform/Interface/IFileReader.h"
#include "CrossPlatform/XenonFile.h"

namespace XenonEnigne {

    using CrossPlatform::IFileReader;
    using CrossPlatform::XenonFile;

    void FileManager::SetFileReader(CrossPlatform::IFileReader*const fileReader)
    {
        m_fileReader = fileReader;
    }

    bool FileManager::Shutdown()
    {
        m_fileReader = nullptr;
        return true;
    }

    XenonFile*const FileManager::ReadFile(const char* const fileName)
    {
       return m_fileReader->ReadFile(fileName);
    }

    Algorithm::String FileManager::GetApplicationPath() const
    {
        return m_fileReader->GetApplicationPath();
    }

}