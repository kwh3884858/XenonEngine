#include "XenonImage.h"
namespace CrossPlatform
{

    XenonImage::XenonImage(unsigned char* image, int height, int width, int channel)
    {
        m_image = image;
        m_height = height;
        m_width = width;
        m_channel = channel;
    }

    XenonImage::~XenonImage()
    {
        delete m_image;
        m_image = nullptr;
    }

}