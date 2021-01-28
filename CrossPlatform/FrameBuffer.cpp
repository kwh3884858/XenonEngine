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

    unsigned int SColorRGB::ToRGBLittleEndian()
    {
        return 0x0 | r | g << 8 | b << 16;
    }

    unsigned char SColorRGB::GetR(int rgb)
    {
        r = (rgb >> 16) & 0xff;
    }

    unsigned char SColorRGB::GetG(int rgb)
    {
        g = (rgb >> 8) & 0xff;
    }

    unsigned char SColorRGB::GetB(int rgb)
    {
        b = rgb & 0xff;
    }

    const int FramerBuffer::UNINITIALUZE_VALUE = -1;

    bool FramerBuffer::InternalCreateBuffer()
    {
        if (!IsResolutionValid()) { return false; }

        if (m_buffer != nullptr)
        {
            ClearBuffer();
        }

        m_buffer = new SColorRGB[m_resolutionX * m_resolutionY];
        return true;
    }

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

    bool FramerBuffer::Initilize(unsigned int resolutionX, unsigned int resolutionY)
    {
        m_resolutionX = resolutionX;
        m_resolutionY = resolutionY;
        InternalCreateBuffer();
    }


    bool FramerBuffer::ClearBuffer()
    {
        if (!IsBufferValid()) { return; }
        if (!IsResolutionValid()) { return; }
        
        delete[] m_buffer;
    }



    unsigned int FramerBuffer::GetColor(unsigned int x, unsigned int y)
    {
        const SColorRGB& scolor = m_buffer[y * m_resolutionX + x];
        return scolor.ToRGBLittleEndian();
    }

}