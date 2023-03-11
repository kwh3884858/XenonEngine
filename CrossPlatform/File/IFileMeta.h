#pragma once
#include "CrossPlatform/File/FileHeader.h"
#include <map>

namespace CrossPlatform
{
	class IFileMeta
	{
	public:
		//using FactoryByPath = IFileMeta * (*)(const Algorithm::String&);
		//using ReaderByHeader = IFileMeta * (*)(const FileHeader&);
		IFileMeta(const FileHeader& header) : m_header(header) { }
		virtual ~IFileMeta() = default;
		const FileHeader& GetFileHeader()const { return m_header; }
		FileHeader& GetFileHeader() { return const_cast<FileHeader&>(static_cast<const IFileMeta&>(*this).GetFileHeader()); }
	public:
		// Add into content browser. create meta for file
		virtual void Add() = 0;

		// Instantiate a new instance
		virtual void* Instantiate() = 0;

		// Clear from memory
		virtual void Clear() = 0;

		// Save to hard drive as a data file
		virtual void Save();

		// Delete data file from hard drive
		virtual void Delete();

	protected:
		FileHeader m_header;

	};
}
