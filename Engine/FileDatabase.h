#pragma once
#include "CrossPlatform/IFile.h"
#include <string>

#include "CrossPlatform/DataPair.h"
#include "CrossPlatform/FileTypeEnum.h"

namespace CrossPlatform
{
    class Folder;
}

namespace XenonEngine
{
    class FileDatabase
    {
    public:
        FileDatabase() = default;

        void Initialize();
        void Shutdown();

        CrossPlatform::FileType GetFileType(const std::string& ext)const;
        const CrossPlatform::Folder* GetDataRoot()const {return m_root; }
    private:
        CrossPlatform::Folder* m_root = nullptr;
        Algorithm::Vector<CrossPlatform::DataPair> m_typePair;
    };
}