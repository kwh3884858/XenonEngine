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
        FileHeader(const FileHeader& header) :
            m_fileType (header.m_fileType),
            m_filePath(header.m_filePath),
            m_guid(header.m_guid)
        {
        }
        FileHeader(FileType fileType, const Algorithm::String& filePath, const xg::Guid& guid) :
            m_fileType(fileType), m_filePath(filePath), m_guid(guid) {}

        FileType GetFileType()const { return m_fileType; }
        FileType SetFileType(FileType fileType) { m_fileType = fileType; }
        const Algorithm::String& GetFilePath()const { return m_filePath; }
        void SetFilePath(Algorithm::String filePath) { m_filePath = filePath; }
        const xg::Guid& GetGUID()const { return m_guid; }
        void SetGUID(xg::Guid guid) { m_guid = guid; }
    private:
        FileType m_fileType = FileType::None;
        Algorithm::String m_filePath;
        xg::Guid m_guid;
    };

    class IFile
    {
    public:
        IFile(const FileHeader& header) : m_header(header) {}

    private:
        FileHeader m_header;
    };

    class Folder :public IFile
    {
    public:
        Folder(const FileHeader& header) :IFile(header) {}
        void AddIFile(IFile* file) { m_content.Add(file); }
    private:
        Algorithm::Vector<IFile*> m_content;
    };

    class Material :public IFile
    {
    public:
        Material(const FileHeader& header) :IFile(header) {}
        CrossPlatform::XenonMaterial* Get() {}
    private:

    };

    class Model :public IFile
    {
    public:
        Model(const FileHeader& header) :IFile(header) {}
        CrossPlatform::XenonMaterial* Get() {}
    private:

    };
}