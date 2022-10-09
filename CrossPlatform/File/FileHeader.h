#pragma once
#include "Algorithms/String.h"
#include "crossguid/guid.hpp"
#include "CrossPlatform/FileTypeEnum.h"

namespace CrossPlatform
{
	class FileHeader
	{
	public:
		friend class IFileMeta;
		static const Algorithm::String Root_Drive;

		FileHeader(FileType fileType = FileType::FileTypeNone) :
			m_fileType(fileType),
			m_guid()
		{}
		FileHeader(const FileHeader& header) :
			m_fileType(header.m_fileType),
			m_filePath(header.m_filePath),
			m_guid(header.m_guid)
		{}
		FileHeader(FileType fileType, const Algorithm::String& filePath, const xg::Guid& guid) :
			m_fileType(fileType), m_filePath(filePath), m_guid(guid) {}

		const FileType GetFileType()const { return m_fileType; }
		void SetFileType(FileType fileType) { m_fileType = fileType; }
		const Algorithm::String& GetFilePath()const { return m_filePath; }
		Algorithm::String GetVirtualPath() const;
		Algorithm::String GetFileName() const;
		void SetFilePath(Algorithm::String filePath) { m_filePath = filePath; }
		const xg::Guid& GetGUID()const { return m_guid; }
		void SetGUID(xg::Guid guid) { m_guid = guid; }

	private:
		void GenerateMetadata()const;

	private:
		FileType m_fileType;
		Algorithm::String m_filePath;
		xg::Guid m_guid;
	};
}
