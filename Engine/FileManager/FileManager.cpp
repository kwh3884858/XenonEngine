#include "Engine/FileManager/FileManager.h"
#include "CrossPlatform/Interface/IFileReader.h"
#include "CrossPlatform/XenonFile.h"
#include "Algorithms/StreamingVector.h"

namespace XenonEnigne {

    using CrossPlatform::IFileReader;
    using CrossPlatform::XenonFile;
    using Algorithm::StreamingVector;

    void FileManager::SetFileReader(CrossPlatform::IFileReader*const fileReader)
    {
        m_fileReader = fileReader;
    }

    bool FileManager::Shutdown()
    {
        m_fileReader = nullptr;
        return true;
    }

    CrossPlatform::XenonFile*const FileManager::ReadFile(const Algorithm::String fileName)const
    {
        return m_fileReader->ReadFile(fileName);
    }

    bool FileManager::WriteFile(const Algorithm::String fileName, StreamingVector<char>*const stream)const
    {
        return m_fileReader->WriteFile(fileName, stream);
    }

    //XenonFile*const FileManager::ReadFile(const char* const fileName)
    //{
    //   return m_fileReader->ReadFile(fileName);
    //}

    Algorithm::String FileManager::GetApplicationPath() const
    {
        return m_fileReader->GetApplicationPath();
    }

}