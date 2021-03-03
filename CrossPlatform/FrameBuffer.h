#pragma once



namespace CrossPlatform
{
#define LOWBYTE(w)  ((unsigned char)(((unsigned long)(w)) & 0xff))
#define HIGHBYTE(w) ((unsigned char)(((unsigned long)(w) >> 8) & 0xff))

    // Union cast is a safer way to do reinterpret_cast<int &>(myfloat) (according to C++ standard)
    template < typename TO, typename FROM >
    inline TO union_cast(FROM value)
    {
        union { FROM from; TO to; } convert;
        convert.from = value;
        return convert.to;
    }

    class SColorRGBA
    {
    public:
        SColorRGBA(int r, int g, int b, int a = 255);
        SColorRGBA(int grb);
        ~SColorRGBA();

        unsigned int ToRGBA();
        unsigned int ToRGB();

    private:
        //unsigned char GetR(int rgba);
        //unsigned char GetG(int rgba);
        //unsigned char GetB(int rgba);
        //unsigned char GetA(int rgba);

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    class SColorRGB
    {
    public:
        SColorRGB();
        SColorRGB(int r, int g, int b);
        SColorRGB(const SColorRGB& rgb);
        ~SColorRGB() = default;

        SColorRGB& operator= (const SColorRGB& rvalue);
        unsigned int ToRGBLittleEndian() const;

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
        bool DeleteBuffer();

        unsigned int GetResolitionX() { return m_resolutionX; }
        unsigned int GetResolitionY() { return m_resolutionY; }

        unsigned int GetColor(unsigned int x, unsigned int y);
        void SetColor(unsigned int x, unsigned int y, const SColorRGB& color);

        float GetZBuffer(unsigned int x, unsigned int y);
        void SetZBuffer(unsigned int x, unsigned int y, const float zBuffer);

        void ClearBuffer();

    private:
        const int UNINITIALUZE_VALUE = -1;
        bool InternalCreateBuffer();
        bool IsResolutionValid()const;
        bool IsBufferValid() const;
        bool IsPositionValid(unsigned int x, unsigned int y)const;

        unsigned int m_resolutionX;
        unsigned int m_resolutionY;

        SColorRGB* m_buffer;
        float* m_zBuffer;
    };

}