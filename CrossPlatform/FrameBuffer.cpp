#include "FrameBuffer.h"

namespace CrossPlatform
{
    float SColorRGBA::ToRGB()
    {
       return ( r | g << 8 | b << 16 );
    }

    SColorRGBA::GetG(int rgba)
    {

    }

    float SColorRGBA::ToRGBA() {
        return (r | g << 8 | b << 16 | a << 24);
    }

    SColorRGB::SColorRGB(int r, int g, int b)
    {
        r = (unsigned char)r;
        g = (unsigned char)g;
        b = (unsigned char)b;
    }

    SColorRGB::SColorRGB(const SColorRGB& rgb)
    {
        r = GetR(rgb);
        g = GetG(rgb);
        b = GetB(rgb);
    }

    SColorRGB& SColorRGB::operation = (const SColorRGB& rvalue)
    {
        r = rvalue.r;
        g = rvalue.g;
        b = rvalue.b;
    }

    float SColorRGB::ToRGB()
    {
        return 0x0 | r | g << 8 | b << 16;
    }

    SColorRGB::GetR(int rgb)
    {
        r = (rgb >> 16) & 0xff;
    }

    SColorRGB::GetG(int rgb)
    {
        g = (rgb >> 8) & 0xff;
    }

    SColorRGB::GetB(int rgb)
    {
        b = rgb & 0xff;
    }

    const int FramerBuffer::UNINITIALUZE_VALUE = -1;

    bool FramerBuffer::IsResolutionValid()
    {
        if (m_resolutionX == UNINITIALUZE_VALUE || m_resolutionY == UNINITIALUZE_VALUE)
        {
            return false;
        }
        return true;
    }

    bool FramerBuffer::IsBufferValid()
    {
        if (m_buffer == nullptr)
        {
            return false;
        }
        return true;
    }

    FramerBuffer::FramerBuffer()
    {
        m_buffer = nullptr;
        m_resolutionX = UNINITIALUZE_VALUE;
        m_resolutionY = UNINITIALUZE_VALUE;
    }

    bool FramerBuffer::CreateBuffer()
    {
        if (!IsResolutionValid()) { return; }

        if (m_buffer != nullptr)
        {
            ClearBuffer();
        }

        m_buffer = new SColorRGB[m_resolutionX * m_resolutionY];
    }


    bool FramerBuffer::ClearBuffer()
    {
        if (!IsBufferValid()) { return; }
        if (!IsResolutionValid()) { return; }
        
        delete[] m_buffer;
    }



}