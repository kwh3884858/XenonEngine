#include "FileMetaFactory.h"
#include "IFileMeta.h"


namespace CrossPlatform
{
	template <class IFileMetaType>
	FileMetaFactory<IFileMetaType> & FileMetaFactory<IFileMetaType>::Instance()
	{
		static FileMetaFactory<IFileMetaType> instance;
		return instance;
	}

	template <class IFileMetaType>
	void CrossPlatform::FileMetaFactory<IFileMetaType>::RegisterProduct(IMetaRegister<IFileMetaType> *registrar, FileType fileType)
	{
		m_ProductRegistry[fileType] = registrar;
	}

	template <class IFileMetaType>
	IFileMetaType * CrossPlatform::FileMetaFactory<IFileMetaType>::GetProduct(const FileHeader& header)
	{
		FileType fileType = header.GetFileType();
		if (m_ProductRegistry.find(fileType) != m_ProductRegistry.end())
		{
			return m_ProductRegistry[fileType]->CreateProduct();
		}
		std::cout << "No meta file found for " << fileType << std::endl;
		return nullptr;
	}
}