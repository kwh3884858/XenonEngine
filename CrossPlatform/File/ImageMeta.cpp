#include "ImageMeta.h"
#include "CrossPlatform/Image/Image.h"
#include "CrossPlatform/Converter/ImageYamlConverter.h"

#include <fstream>
#include <filesystem>

namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;

	void ImageMeta::Load()
	{
		if (!m_image)
		{
			YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
			*m_image = std::move(config.as<Image>());
		}
	}

	void ImageMeta::Clear()
	{
		delete m_image;
		m_image = nullptr;
	}

	void ImageMeta::Save()
	{
		IFileMeta::Save();

		ofstream outputStream(GetFileHeader().GetFilePath().CString());
		YAML::Emitter out(outputStream);
		out << YAML::Node(*m_image);
		outputStream.close();
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

	CrossPlatform::Image* ImageMeta::GetImage() 
	{
		if (!m_image)
		{
			m_image = new Image(GetFileHeader().GetFilePath());
		}
		return m_image;
	}


}