#include "CrossPlatform/SColorRGBA.h"

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
        value = 0;
    }

    SColorRGBA::SColorRGBA(int b, int g, int r, int a /*= 255*/)
    {
        value = (b | g << 8 | r << 16 | a << 24);
    }

    SColorRGBA::SColorRGBA(int rgba)
    {
        value = rgba;
    }

    SColorRGBA::SColorRGBA(const SColorRGBA& rhs)
    {
        this->value = rhs.value;
    }

    SColorRGBA::~SColorRGBA()
    {

    }

    SColorRGBA& SColorRGBA::operator=(const SColorRGBA& rhs)
    {
        this->value = rhs.value;
        return *this;
    }

    unsigned int SColorRGBA::ToRGBALittleEndian()
    {
        return value;
    }

    unsigned int SColorRGBA::ToRGBLittleEndian()
    {
        return value & 0xffffff;
    }
}