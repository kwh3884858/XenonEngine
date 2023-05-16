#pragma once
#include "CrossPlatform/XenonObject.h"
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
		// If a file have not a related meta: It is a new file, create a meta for it, import the data and put them into folder.
		// if a file have a related meta: it is a processed file, dont do anything just notice content brower that we found a file.

		// Trigger after load meta file into content browser
		virtual void OnLoad() {}

		// Trigger after import into engine folder
		virtual void OnImport() {}

		// For every founded file, put it into game world should call this function.
		// It will return a instance pointer.
		// And due to it will return a instance, so it doesn`t need to contain any data/data pointer in meta file.
		// Instantiate a new instance
		virtual XenonObject* Instantiate() const = 0;

		// Clear from memory
		virtual void Clear() = 0;

		// Save to hard drive as a data file
		virtual void Save(const XenonObject* data = nullptr);

		// Delete data file from hard drive
		virtual void Delete();

	protected:
		FileHeader m_header;

	};
}
