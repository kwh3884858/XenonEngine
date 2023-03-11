#pragma once
//#include "CrossPlatform/FileTypeEnum.h"
#include "CrossPlatform/File/FileHeader.h"
//#include "IFileMeta.h"
#include <map>
#include "IMetaRegister.h"
namespace CrossPlatform
{
	//template <class IFileMetaType>
	//class IMetaRegister;

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

	template <class IFileMetaType>
	FileMetaFactory<IFileMetaType>& FileMetaFactory<IFileMetaType>::Instance()
	{
		static FileMetaFactory<IFileMetaType> instance;
		return instance;
	}

	template <class IFileMetaType>
	void CrossPlatform::FileMetaFactory<IFileMetaType>::RegisterProduct(IMetaRegister<IFileMetaType>* registrar, FileType fileType)
	{
		m_ProductRegistry[fileType] = registrar;
	}

	template <class IFileMetaType>
	IFileMetaType* CrossPlatform::FileMetaFactory<IFileMetaType>::GetProduct(const FileHeader& header)
	{
		FileType fileType = header.GetFileType();
		if (m_ProductRegistry.find(fileType) != m_ProductRegistry.end())
		{
			return m_ProductRegistry[fileType]->CreateProduct(header);
		}
		assert(true == false, "Check Type!");
		std::cout << "No meta file found for " << (int) fileType << std::endl;
		return nullptr;
	}

}