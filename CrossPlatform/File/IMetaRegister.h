#pragma once

namespace CrossPlatform
{
	class FileHeader;
	template <class IFileMetaType>
	class IMetaRegister
	{
	public:
		virtual IFileMetaType *CreateProduct(const FileHeader& header) = 0;

	protected:
		IMetaRegister() = default;
		virtual ~IMetaRegister() = default;

	private:
		IMetaRegister(const IMetaRegister &) = default;
		IMetaRegister& operator=(const IMetaRegister &) = default;
	};
}