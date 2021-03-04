#include "FrameBuffer.h"
#include <cstring>
#include <Windows.h>
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
        this->r = (unsigned char)r;
        this->g = (unsigned char)g;
        this->b = (unsigned char)b;
    }

    SColorRGB::SColorRGB(const SColorRGB& rgb)
    {
        this->r = rgb.r;
        this->g = rgb.g;
        this->b = rgb.b;
    }

    SColorRGB::SColorRGB()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }

    SColorRGB& SColorRGB::operator=(const SColorRGB& rvalue)
    {
        this->r = rvalue.r;
        this->g = rvalue.g;
        this->b = rvalue.b;
        return *this;
    }

    unsigned long SColorRGB::ToRGBLittleEndian() const
    {
        //return 0x0 | r | g << 8 | b << 16;
        return RGB(this->r, this->g, this->b);
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
        m_zBuffer = new float[m_resolutionX * m_resolutionY];
        return true;
    }

    bool FramerBuffer::IsResolutionValid()const
    {
        if (m_resolutionX == UNINITIALUZE_VALUE || m_resolutionY == UNINITIALUZE_VALUE)
        {
            return false;
        }
        return true;
    }

    bool FramerBuffer::IsBufferValid()const 
    {
        if (m_buffer == nullptr)
        {
            return false;
        }
        if (m_zBuffer == nullptr)
        {
            return false;
        }
        return true;
    }

    bool FramerBuffer::IsPositionValid(unsigned int x, unsigned int y) const
{
        if (x < 0 || x >= m_resolutionX || y < 0 || y >= m_resolutionY)
        {
            return false;
        }
        return true;
    }

    FramerBuffer::FramerBuffer()
    {
        m_buffer = nullptr;
        m_zBuffer = nullptr;
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
        delete[] m_zBuffer;
        return true;
    }

    unsigned long FramerBuffer::GetColor(unsigned int x, unsigned int y)
    {
        if (!IsPositionValid(x, y)) { return 0; }
        const SColorRGB& scolor = m_buffer[y * m_resolutionX + x];
        return scolor.ToRGBLittleEndian();
    }

    void FramerBuffer::SetColor(unsigned int x, unsigned int y, const SColorRGB& color)
    {
        if (!IsPositionValid(x, y)) { return; }
        m_buffer[y * m_resolutionX + x] = color;
    }

    float FramerBuffer::GetZBuffer(unsigned int x, unsigned int y)
    {
        if (!IsPositionValid(x, y)) { return 0; }
        return m_zBuffer[y * m_resolutionX + x];
    }

    void FramerBuffer::SetZBuffer(unsigned int x, unsigned int y, const float zBuffer)
    {
        if (!IsPositionValid(x, y)) { return; }
        m_zBuffer[y * m_resolutionX + x] = zBuffer;
    }

    void FramerBuffer::ClearBuffer()
    {
        if (!IsBufferValid()) { return ; }
        if (!IsResolutionValid()) { return ; }
        int size = sizeof(SColorRGB);
        int zBufferSize = sizeof(float);
        memset(m_buffer, 0, m_resolutionX*m_resolutionY*size);
        memset(m_zBuffer, 0, m_resolutionX*m_resolutionY*zBufferSize);
    }

}