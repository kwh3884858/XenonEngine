#include "Image.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb/stb_image.h"

namespace CrossPlatform
{
	using namespace MathLab;
	Image::Image(const Algorithm::String& fileName)
	{
		m_data = stbi_load(fileName.CString(), &m_width, &m_height, &m_channel, 0);
	}

	Image::Image(const Image& that)
	{
		int size = that.m_height*m_width*m_channel;
		m_data = new unsigned char[size];
		memcpy(m_data, that.m_data, size);
	}

	Image::~Image()
	{
		delete m_data;
		m_data = nullptr;
		m_width = -1;
		m_height = -1;
		m_channel = -1;
	}

	MathLab::Vector4f Image::GetColor(int x, int y) const
	{
		assert(x >= 0 && x < m_width);
		assert(y >= 0 && y < m_height);
		Vector4f result;
		result.x = m_data[x * m_width + y];
		result.y = m_data[x * m_width + y + 1];
		result.z = m_data[x * m_width + y + 2];
		if (m_channel == 3)
		{
			result.w = 255;
		}
		else if(m_channel == 4)
		{
			result.w = m_data[x * m_width + y + 3];
		}
		return result;
	}

	MathLab::Vector4f Image::GetColor(float x, float y) const
	{
		return GetColor((int)x, (int)y);
	}

}