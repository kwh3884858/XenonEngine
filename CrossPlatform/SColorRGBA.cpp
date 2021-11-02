#include "CrossPlatform/SColorRGBA.h"
#include <assert.h>

namespace CrossPlatform
{
    //unsigned int SColorRGBA::ToRGB()
    //{
    //    return (r | g << 8 | b << 16);
    //}

    //unsigned int SColorRGBA::ToRGBA() {
    //    return (r | g << 8 | b << 16 | a << 24);
    //}

    SColorRGBA::SColorRGBA()
    {
        m_value = 255 << 24;
    }

    SColorRGBA::SColorRGBA(int r, int g, int b, int a /*= 255*/)
    {
        assert(r >= 0 && r < COLORLIMIT);
        assert(g >= 0 && g < COLORLIMIT);
        assert(b >= 0 && b < COLORLIMIT);
        assert(a >= 0 && a < COLORLIMIT);
        m_value = (b | g << 8 | r << 16 | a << 24);
    }

    SColorRGBA::SColorRGBA(int rgba)
    {
        m_value = rgba;
    }

    SColorRGBA::SColorRGBA(const SColorRGBA& rhs)
    {
        this->m_value = rhs.m_value;
    }

    SColorRGBA::~SColorRGBA()
    {

    }

    SColorRGBA& SColorRGBA::operator=(const SColorRGBA& rhs)
    {
        this->m_value = rhs.m_value;
        return *this;
    }

    SColorRGBA SColorRGBA::operator+(const SColorRGBA& rhs)
    {
        SColorRGBA result(*this);
        return result += rhs.m_value;
    }

    SColorRGBA& SColorRGBA::operator+=(const SColorRGBA& rhs)
    {
        int r = (*this).GetR();
        int g = (*this).GetG();
        int b = (*this).GetB();
        r = r + rhs.GetR();
        r = r >= COLORLIMIT ? COLORLIMIT - 1 : r;
        g = g + rhs.GetG();
        g = g >= COLORLIMIT ? COLORLIMIT - 1 : g;
        b = b + rhs.GetB();
        b = b >= COLORLIMIT ? COLORLIMIT - 1 : b;
        SetR(r);
        SetG(g);
        SetB(b);
        return *this;
    }

    //Do Not Protect Overflow
    SColorRGBA SColorRGBA::operator-(const SColorRGBA& rhs)
    {
        int r = (*this).GetR();
        int g = (*this).GetG();
        int b = (*this).GetB();
        r = rhs.GetR() >= r ? 0 : r - rhs.GetR();
        g = rhs.GetG() >= g ? 0 : g - rhs.GetG();
        b = rhs.GetB() >= b ? 0 : b - rhs.GetB();
        SetR(r);
        SetG(g);
        SetB(b);
        return *this;
    }

    SColorRGBA SColorRGBA::operator*(const SColorRGBA& rhs) const
    {
        float r = (*this).GetR();
        float g = (*this).GetG();
        float b = (*this).GetB();
        r = r * rhs.GetR() / COLORLIMIT;
        g = g * rhs.GetG() / COLORLIMIT;
        b = b * rhs.GetB() / COLORLIMIT;
        SColorRGBA result(r, g, b);
        return result;
    }

    SColorRGBA SColorRGBA::operator*(float rhs) const
    {
        int r = (*this).GetR();
        int g = (*this).GetG();
        int b = (*this).GetB();
        r = r * rhs;
        r = r >= COLORLIMIT ? COLORLIMIT - 1 : r;
        g = g * rhs;
        g = g >= COLORLIMIT ? COLORLIMIT - 1 : g;
        b = b * rhs;
        b = b >= COLORLIMIT ? COLORLIMIT - 1 : b;
        SColorRGBA result(r, g, b);
        return result;
    }

    SColorRGBA SColorRGBA::operator/(float rhs) const
    {
        int r = (*this).GetR();
        int g = (*this).GetG();
        int b = (*this).GetB();
        r = r / rhs;
        r = r >= COLORLIMIT ? COLORLIMIT - 1 : r;
        g = g / rhs;
        g = g >= COLORLIMIT ? COLORLIMIT - 1 : g;
        b = b / rhs;
        b = b >= COLORLIMIT ? COLORLIMIT - 1 : b;
        SColorRGBA result(r, g, b);
        return result;
    }

    unsigned int SColorRGBA::ToRGBALittleEndian()
    {
        return m_value;
    }

    unsigned int SColorRGBA::ToRGBLittleEndian()
    {
        return m_value & 0xffffff;
    }

    void SColorRGBA::SetB(int value)
    {
        assert(value >= 0 && value < COLORLIMIT);
        int mask = ~0 & ~0xff;
        m_value &= mask;
        m_value |= value;
    }

    void SColorRGBA::SetG(int value)
    {
        assert(value >= 0 && value < COLORLIMIT);
        int mask = ~0 & ~(0xff << 8);
        m_value &= mask;
        m_value |= (value<<8);
    }

    void SColorRGBA::SetR(int value)
    {
        assert(value >= 0 && value < COLORLIMIT);
        int mask = ~0 & ~(0xff << 16);
        m_value &= mask;
        m_value |= (value << 16);
    }

    void SColorRGBA::SetA(int value)
    {
        assert(value >= 0 && value < COLORLIMIT);
        int mask = ~0 & ~(0xff << 24);
        m_value &= mask;
        m_value |= (value << 24);
    }

    int SColorRGBA::COLORLIMIT = 256;

}