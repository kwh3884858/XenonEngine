#include "ImageMeta.h"
#include "CrossPlatform/Image/Image.h"
#include <filesystem>
namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;
	ImageMeta::~ImageMeta()
    {
		Clear();
    }

	void ImageMeta::Delete()
	{
		Clear();
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
		{
			String metaFilePath = filePath + ".metadata";
			path modelMetaFile(metaFilePath.CString());
			bool result = remove(modelMetaFile);
			assert(result == true);
		}
	}

	CrossPlatform::Image* ImageMeta::GetImage() 
	{
		if (!m_image)
		{
			m_image = new Image(GetFileHeader().GetFilePath());
		}
		return m_image;
	}

	void ImageMeta::Clear()
	{
		delete m_image;
		m_image = nullptr;
	}

}