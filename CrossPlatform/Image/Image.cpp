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

	MathLab::TVector4f Image::GetColor(int x, int y) const
	{
		TVector4f result;
		result[0] = m_data[x * m_width + y];
		result[1] = m_data[x * m_width + y + 1];
		result[2] = m_data[x * m_width + y + 2];
		if (m_channel == 3)
		{
			result[3] = 255;
		}
		else if(m_channel == 4)
		{
			result[3] = m_data[x * m_width + y + 3];
		}
		return result;
	}

}