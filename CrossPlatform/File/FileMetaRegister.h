#pragma once
#include "CrossPlatform/File/FileMetaFactory.h"
#include "CrossPlatform/File/IMetaRegister.h"

namespace CrossPlatform
{
	template <class ProductType_t, class ProductImpl_t>
	class FileMetaRegister : public IMetaRegister<ProductType_t>
	{
	public:
		explicit FileMetaRegister(FileType fileType)
		{
			FileMetaFactory<ProductType_t>::Instance().RegisterProduct(this, fileType);
		}

		ProductType_t *CreateProduct(const FileHeader& header)
		{
			return new ProductImpl_t(header);
		}
	};
}