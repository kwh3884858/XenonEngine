#include "ImageMeta.h"
#include "CrossPlatform/Image/Image.h"
#include "CrossPlatform/Converter/ImageYamlConverter.h"

#include <fstream>
#include <filesystem>
#include "Library/stb/stb_image.h"

namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;

	//void ImageMeta::Add()
	//{
		//m_data = stbi_load(fileName.CString(), &m_width, &m_height, &m_channel, 0);
		//assert(m_data != nullptr);
		//if (!m_image)
		//{

		//}
	//}

	Image* ImageMeta::Instantiate()
	{
		return ImportImageFileIntoFolderAsNativeVersion();
	}

	void ImageMeta::Clear()
	{
		//delete m_image;
		//m_image = nullptr;
	}

	void ImageMeta::Save(const XenonObject* data /*= nullptr*/)
	{
		IFileMeta::Save();
		//ImportImageFileIntoFolderAsNativeVersion();
	}

	void ImageMeta::Delete()
	{
		IFileMeta::Delete();

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			path modelFile(filePath.CString());
			bool result = remove(modelFile);
			assert(result == true);
		}
	}

	Image* ImageMeta::ImportImageFileIntoFolderAsNativeVersion() const
	{
		int width, height, channel;
		unsigned char* const data = stbi_load(GetFileHeader().GetFilePath().CString(), &width, &height, &channel, 0);
		assert(data != nullptr);

		return new Image(data, height, width, channel);
	}

	//CrossPlatform::Image* ImageMeta::GetImage()
	//{
	//	if (!m_image)
	//	{
	//		m_image = new Image(GetFileHeader().GetFilePath());
	//	}
	//	return m_image;
	//}


}