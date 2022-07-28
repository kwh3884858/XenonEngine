#pragma once
#include "Algorithms/String.h"
#include "crossguid/guid.hpp"
#include "CrossPlatform/FileTypeEnum.h"
namespace CrossPlatform
{
    class Material;

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
		using FactoryByPath = IFileMeta* (*)(const Algorithm::String& );
		using ReaderByHeader = IFileMeta * (*)(const FileHeader&);
		IFileMeta(const FileHeader& header) : m_header(header) { }
        virtual ~IFileMeta() {};
        const FileHeader& GetFileHeader()const { return m_header; }

	public:
		virtual void Load() = 0;
		virtual void Save() = 0;
		virtual void Delete() = 0;

	protected:
		FileHeader m_header;

	public:
		static IFileMeta* CreateNewFileMeta(FileType fileType, const Algorithm::String& filePath) { return s_factory[fileType](filePath); }
		static IFileMeta* CreateNewFileMeta(FileType fileTyoe, const FileHeader& fileHeader) { return s_loader[fileTyoe](fileHeader); }
    protected:
		static void RegisterFileFacotry(FileType fileType, FactoryByPath factory)
		{
			s_factory.insert(std::pair<FileType, FactoryByPath>(fileType, factory));
		}
		static void RegisterFileLoader(FileType fileType, ReaderByHeader loader)
		{
			s_loader.insert(std::pair<FileType, ReaderByHeader>(fileType, loader));
		}
    private:
		static std::map<FileType, FactoryByPath> s_factory;
		static std::map<FileType, ReaderByHeader> s_loader;
		//static std::map<FileType, FactoryByPath>& Lookup() {
	//	static std::map<FileType, FactoryByPath> factory;
	//	return factory;
	//}
	};
}