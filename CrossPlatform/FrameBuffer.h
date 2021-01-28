#pragma once



namespace CrossPlatform
{
#define LOWBYTE(w)  ((unsigned char)(((unsigned long)(w)) & 0xff))
#define HIGHBYTE(w) ((unsigned char)(((unsigned long)(w) >> 8) & 0xff))

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

        SColorRGB& operation= (const SColorRGB& rvalue);

        ~SColorRGB() = default;
        unsigned int ToRGBLittleEndian();

    private:
        unsigned char GetR(int rgbr);
        unsigned char GetG(int rgbg);
        unsigned char GetB(int rgbb);

        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    //template<int ResolutionX = UNINITIALUZE_VALUE, int ResolutionY = UNINITIALUZE_VALUE>
    class FramerBuffer
    {
    public:
        FramerBuffer();
        virtual ~FramerBuffer() = default;

        bool Initilize(unsigned int resolutionX, unsigned int resolutionY);
        bool ClearBuffer();

        unsigned int GetResolitionX() { return m_resolutionX; }
        unsigned int GetResolitionY() { return m_resolutionY; }
        unsigned int GetColor(unsigned int x, unsigned int y);

    private:
        const int UNINITIALUZE_VALUE;
        bool InternalCreateBuffer();
        bool IsResolutionValid();
        bool IsBufferValid();

        unsigned int m_resolutionX;
        unsigned int m_resolutionY;

        SColorRGB* m_buffer;
    };

}