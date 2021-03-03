#include "FrameBuffer.h"
#include <cstring>

namespace CrossPlatform
{
    unsigned int SColorRGBA::ToRGB()
    {
        return (r | g << 8 | b << 16);
    }

    unsigned int SColorRGBA::ToRGBA() {
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
        r = rgb.r;
        g = rgb.g;
        b = rgb.b;
    }

    SColorRGB::SColorRGB()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    SColorRGB& SColorRGB::operator=(const SColorRGB& rvalue)
    {
        r = rvalue.r;
        g = rvalue.g;
        b = rvalue.b;
        return *this;
    }

    unsigned int SColorRGB::ToRGBLittleEndian() const
    {
        return 0x0 | r | g << 8 | b << 16;
    }

    unsigned char SColorRGB::GetR(int rgb)
    {
        return static_cast<unsigned char>((rgb >> 16) & 0xff);
    }

    unsigned char SColorRGB::GetG(int rgb)
    {
        return static_cast<unsigned char>((rgb >> 8) & 0xff);
    }

    unsigned char SColorRGB::GetB(int rgb)
    {
        return static_cast<unsigned char>(rgb & 0xff);
    }

    bool FramerBuffer::InternalCreateBuffer()
    {
        if (!IsResolutionValid()) { return false; }

        if (m_buffer != nullptr)
        {
            DeleteBuffer();
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
        return true;
    }

    bool FramerBuffer::DeleteBuffer()
    {
        if (!IsBufferValid()) { return false; }
        if (!IsResolutionValid()) { return false; }

        delete[] m_buffer;
        return true;
    }

    unsigned int FramerBuffer::GetColor(unsigned int x, unsigned int y)
    {
        const SColorRGB& scolor = m_buffer[y * m_resolutionX + x];
        return scolor.ToRGBLittleEndian();
    }

    void FramerBuffer::SetColor(unsigned int x, unsigned int y, const SColorRGB& color)
    {
        if (x <0 || x >= m_resolutionX || y < 0 || y >= m_resolutionY)
        {
            return;
        }
        m_buffer[y * m_resolutionX + x] = color;
    }

    void FramerBuffer::ClearBuffer()
    {
        if (!IsBufferValid()) { return ; }
        if (!IsResolutionValid()) { return ; }
        int size = sizeof(SColorRGB);
        memset(m_buffer, 0, m_resolutionX*m_resolutionY*size);

    }

}