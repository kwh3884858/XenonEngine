//  SColorRGBA.h
//  MacWindow
//
//  Created by whkong on 2021-3-22.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
namespace CrossPlatform
{
	class SColorRGBA
	{
	public:
        SColorRGBA();
		SColorRGBA(int r, int g, int b, int a = 255);
		SColorRGBA(int rgba);
        SColorRGBA(const SColorRGBA& rhs);
		~SColorRGBA();

        SColorRGBA& operator=(const SColorRGBA& rhs);
        SColorRGBA operator+(const SColorRGBA& rhs);
        SColorRGBA& operator+=(const SColorRGBA& rhs);
        SColorRGBA operator*(const SColorRGBA& rhs) const;

		unsigned int ToRGBALittleEndian();
		unsigned int ToRGBLittleEndian();

        unsigned char GetB()const { return static_cast<unsigned char>(0xff & m_value); }
        unsigned char GetG()const { return static_cast<unsigned char>(0xff & (m_value >> 8)); }
        unsigned char GetR()const { return static_cast<unsigned char>(0xff & (m_value >> 16)); }
        unsigned char GetA()const { return static_cast<unsigned char>(0xff & (m_value >> 24)); }


        void SetB(int value);
        void SetG(int value);
        void SetR(int value);
        void SetA(int value);

	private:
        static int COLORLIMIT;
		unsigned int m_value;
	};
    const SColorRGBA WHITE(255, 255, 255, 255);
    const SColorRGBA BLUE(0, 0, 255, 255);
    const SColorRGBA DRAK_BLUE(0, 0, 139, 255);
    const SColorRGBA GREEN(0, 255, 0, 255);
    const SColorRGBA DARK_GREEN(0, 100, 0, 255);
    const SColorRGBA RED(255, 0, 0,  255);
    const SColorRGBA ORANGE_RED( 255, 69, 0, 255);
    const SColorRGBA PALE_GODENROD( 238, 232, 170, 255);
    const SColorRGBA INDIAN_RED(205, 92, 92, 255);
    const SColorRGBA YELLOW(255, 255, 0, 255);

}