#include "DataPair.h"

namespace CrossPlatform
{
    DataPair::DataPair(FileType type, const Algorithm::String& fileSuffix) :
        m_fileType(type),
        m_fileSuffix(fileSuffix) {}

    DataPair::DataPair(const DataPair& data)
    {
        m_fileType = data.m_fileType;
        m_fileSuffix = data.m_fileSuffix;
    }


}