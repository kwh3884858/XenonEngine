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

    SColorRGBA::SColorRGBA(int r, int g, int b, int a /*= 255*/)
    {
        value = (r | g << 8 | b << 16 | a << 24);
    }

    SColorRGBA::SColorRGBA(int rgba)
    {
        value = rgba;
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