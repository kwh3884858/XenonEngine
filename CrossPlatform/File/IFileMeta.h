#pragma once
#include "Algorithms/String.h"
#include "crossguid/guid.hpp"
#include "CrossPlatform/FileTypeEnum.h"
namespace CrossPlatform
{
    class XenonMaterial;

    class FileHeader
    {
    public:
        static const Algorithm::String Root_Drive;

        FileHeader():m_fileType(FileType::None),m_guid() {}
        FileHeader(const FileHeader& header) :
            m_fileType (header.m_fileType),
            m_filePath(header.m_filePath),
            m_guid(header.m_guid)
        {
        }
        FileHeader(FileType fileType, const Algorithm::String& filePath, const xg::Guid& guid) :
            m_fileType(fileType), m_filePath(filePath), m_guid(guid) {}

        FileType GetFileType()const { return m_fileType; }
        void SetFileType(FileType fileType) { m_fileType = fileType; }
        const Algorithm::String& GetFilePath()const { return m_filePath; }
        Algorithm::String GetVirtualPath() const;
        Algorithm::String GetFileName() const;
        void SetFilePath(Algorithm::String filePath) { m_filePath = filePath; }
        const xg::Guid& GetGUID()const { return m_guid; }
        void SetGUID(xg::Guid guid) { m_guid = guid; }

        void GenerateMetadata()const;
    private:
        FileType m_fileType = FileType::None;
        Algorithm::String m_filePath;
        xg::Guid m_guid;
    };

    class IFileMeta
    {
    public:
        IFileMeta(const FileHeader& header) : m_header(header) {}
        virtual ~IFileMeta() {};
		virtual void Delete() = 0;
        const FileHeader& GetFileHeader()const { return m_header; }
    protected:
        FileHeader m_header;
    private:
    };

    class MaterialMeta :public IFileMeta
    {
    public:
        MaterialMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeMaterial); }
        CrossPlatform::XenonMaterial* Get() {}
		virtual void Delete()override {}
    private:

    };
}