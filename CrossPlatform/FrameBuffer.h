#pragma once

typedef FramerBuffer* FramerBufferHandler;

namespace CrossPlatform
{

    class SColorRGBA
    {
    public:
        SColorRGBA(int r, int g, int b, int a = 255);
        SColorRGBA(int grb);
        ~SColorRGBA();

        float ToRGBA();
        float ToRGB();

    private:
        GetR(int rgba);
        GetG(int rgba);
        GetB(int rgba);
        GetA(int rgba);

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    class SColorRGB
    {
    public:
        SColorRGB(int r, int g, int b);
        SColorRGB(const SColorRGB& rgb);

        SColorRGB& operation= (const SColorRGB& lvalue);

        ~SColorRGB();
        float ToRGB();

    private:
        GetR(int rgbr);
        GetG(int rgbg);
        GetB(int rgbb);

        unsigned char r;
        unsigned char g;
        unsigned char b;
    };


    class FramerBuffer
    {
    public:
        FramerBuffer();
        virtual ~FramerBuffer() = default;

        bool CreateBuffer();
        bool ClearBuffer();

    private:
        const int UNINITIALUZE_VALUE;
        void InternalCreateBuffer();
        bool IsResolutionValid();
        bool IsBufferValid();

        unsigned int m_resolutionX;
        unsigned int m_resolutionY;

        SColorRGB* m_buffer;
    };

}