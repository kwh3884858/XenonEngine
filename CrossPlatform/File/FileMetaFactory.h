#pragma once
#include "CrossPlatform/FileTypeEnum.h"
#include "CrossPlatform/File/FileHeader.h"
#include <map>
namespace CrossPlatform
{
	template <class IFileMetaType>
	class IMetaRegister;

	template <class IFileMetaType>
	class FileMetaFactory
	{
	public:
		static FileMetaFactory<IFileMetaType> &Instance();

		void RegisterProduct(IMetaRegister<IFileMetaType> *registrar, FileType fileType);
		IFileMetaType* GetProduct(const FileHeader& header);

	private:
		FileMetaFactory() {}
		~FileMetaFactory() {}
		FileMetaFactory(const FileMetaFactory &) {}
		const FileMetaFactory &operator=(const FileMetaFactory &) {}

		std::map<FileType, IMetaRegister<IFileMetaType> *> m_ProductRegistry;
	};
}