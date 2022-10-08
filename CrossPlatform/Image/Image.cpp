#include "Image.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb/stb_image.h"
#include "MathLab/Vector4.h"

namespace CrossPlatform
{
	using namespace MathLab;
	Image::Image(const Algorithm::String& fileName)
	{
		m_data = stbi_load(fileName.CString(), &m_width, &m_height, &m_channel, 0);
		assert(m_data != nullptr);
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

	CrossPlatform::Image& Image::operator=(Image&& that)
	{
		m_data = that.m_data;
		m_width = that.m_width;
		m_height = that.m_height;
		m_channel = that.m_channel;

		that.m_data = nullptr;
		return *this;
	}

	CrossPlatform::SColorRGBA Image::GetColor(int x, int y) const
	{
		assert(m_data != nullptr);
		assert(x >= 0 && x < m_width);
		assert(y >= 0 && y < m_height);
		Vector4f result;
		result.x = m_data[(x * m_width + y) * m_channel];
		result.y = m_data[(x * m_width + y) * m_channel + 1];
		result.z = m_data[(x * m_width + y) * m_channel + 2];
		if (m_channel == 3)
		{
			result.w = 255;
		}
		else if(m_channel == 4)
		{
			result.w = m_data[(x * m_width + y) * m_channel + 3];
		}
		result /= 255;
		return result.ToColor();
	}

	CrossPlatform::SColorRGBA Image::GetColor(float x, float y) const
	{
		assert(x >= 0 && x <= 1);
		assert(y >= 0 && y <= 1);
		int extendX = (int)( x * m_width);
		int extendY = (int)( y * m_height);
		return GetColor(extendX, extendY);
	}

}