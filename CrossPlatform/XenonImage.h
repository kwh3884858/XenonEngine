#pragma once

namespace CrossPlatform 
{
    class XenonImage
    {
    public:
        XenonImage(unsigned char* image, int height, int width, int channel);
        ~XenonImage();

    private:
        unsigned char* m_image = nullptr;
        int m_height = -1;
        int m_width = -1;
        int m_channel = 4;
    };
}
