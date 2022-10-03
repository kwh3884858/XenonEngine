#pragma once
#include "Algorithms/String.h"
#include "crossguid/guid.hpp"
#include "CrossPlatform/FileTypeEnum.h"
#include <map>

namespace CrossPlatform
{
    class Material;

    class FileHeader
    {
    public:
        friend class IFileMeta;
        static const Algorithm::String Root_Drive;

        FileHeader(FileType fileType):
			m_fileType(fileType),
			m_guid() 
		{}
        FileHeader(const FileHeader& header) :
            m_fileType (header.m_fileType),
            m_filePath(header.m_filePath),
            m_guid(header.m_guid)
        {}
        FileHeader(FileType fileType, const Algorithm::String& filePath, const xg::Guid& guid) :
            m_fileType(fileType), m_filePath(filePath), m_guid(guid) {}

		constexpr FileType GetFileType()const { return m_fileType; }
        //void SetFileType(FileType fileType) { m_fileType = fileType; }
        const Algorithm::String& GetFilePath()const { return m_filePath; }
        Algorithm::String GetVirtualPath() const;
        Algorithm::String GetFileName() const;
        void SetFilePath(Algorithm::String filePath) { m_filePath = filePath; }
        const xg::Guid& GetGUID()const { return m_guid; }
        void SetGUID(xg::Guid guid) { m_guid = guid; }

    private:
		void GenerateMetadata()const;

    private:
        const FileType m_fileType;
        Algorithm::String m_filePath;
        xg::Guid m_guid;
    };

    class IFileMeta
    {
    public:
		using FactoryByPath = IFileMeta* (*)(const Algorithm::String& );
		using ReaderByHeader = IFileMeta * (*)(const FileHeader&);
		IFileMeta(const FileHeader& header) : m_header(header) { }
        virtual ~IFileMeta() = default;
        const FileHeader& GetFileHeader()const { return m_header; }

	public:
        // Load into memory
		virtual void Load() = 0;

        // Clear from memory
        virtual void Clear() = 0;

		// Save to hard drive as a data file
		virtual void Save();

        // Delete data file from hard drive
		virtual void Delete();

	protected:
		FileHeader m_header;

	public:
        template<typename T>
        static void RegisterNewFileType() {
            typename T::
			s_factory.insert(std::pair<FileType, FactoryByPath>(fileType, factory));

        }

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

	template<FileType fileType>
	IFileMeta* CreateFileMetaFromHeader(const FileHeader& fileHeader) { return nullptr; }

	//template<> 
 //   IFileMeta* CreateFileMetaFromHeader<FileType::FileTypeFolder>(const FileHeader& fileHeader)
	//{
 //       CrossPlatform::FolderMeta* meta = new CrossPlatform::FolderMeta(fileHeader);
	//	return meta; 
	//} 

#define DEFINE_FILE_TYPE(Type, FileTypeClass) \
    template<> \
    IFileMeta* CreateFileMetaFromHeader<Type>(const FileHeader& fileHeader) \
    { \
	    FileTypeClass* meta = new FileTypeClass(fileHeader); \
	    return meta; \
    } \

}