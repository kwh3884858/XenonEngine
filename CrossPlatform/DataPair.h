#pragma once

namespace CrossPlatform
{
    struct DataPair
    {
    public:
        DataPair(FileType type, const Algorithm::String& fileSuffix) :
            m_fileType(type),
            m_fileSuffix(fileSuffix) {}
        DataPair(const DataPair& data);
        m_fileType = data.m_fileType;
        m_fileSuffix = data.m_fileSuffix;

        FileType m_fileType;
        Algorithm::String m_fileSuffix;
    };
}