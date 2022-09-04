#pragma once
#include "CrossPlatform/FileTypeEnum.h"
#include "Algorithms/String.h"

namespace CrossPlatform
{
    struct DataPair
    {
    public:
        DataPair() = default;
        DataPair(FileType type, const Algorithm::String& fileSuffix);
        DataPair(const DataPair& data);

        FileType m_fileType = FileType::FileTypeNone;
        Algorithm::String m_fileSuffix;
    };
}