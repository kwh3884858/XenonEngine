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
		SColorRGBA(int b, int g, int r, int a = 255);
		SColorRGBA(int rgba);
        SColorRGBA(const SColorRGBA& rhs);
		~SColorRGBA();

        SColorRGBA& operator=(const SColorRGBA& rhs);

		unsigned int ToRGBALittleEndian();
		unsigned int ToRGBLittleEndian();

        unsigned char GetR()const { return static_cast<unsigned char>(0xff & value); }
        unsigned char GetG()const { return static_cast<unsigned char>(0xff & (value >> 8)); }
        unsigned char GetB()const { return static_cast<unsigned char>(0xff & (value >> 16)); }
        unsigned char GetA()const { return static_cast<unsigned char>(0xff & (value >> 24)); }

	private:

		unsigned int value;
	};
    const SColorRGBA WHITE(255, 255, 255, 255);
    const SColorRGBA BLUE(255, 0, 0, 255);
    const SColorRGBA DRAK_BLUE(139, 0, 0, 255);
    const SColorRGBA GREEN(0, 255, 0, 255);
    const SColorRGBA DARK_GREEN(0, 100, 0, 255);
    const SColorRGBA RED(0, 0, 255, 255);
    const SColorRGBA ORANGE_RED(0, 69, 255, 255);
    const SColorRGBA PALE_GODENROD(170, 232, 238, 255);
    const SColorRGBA INDIAN_RED(92, 92, 205, 255);
    const SColorRGBA YELLOW(0, 255, 255, 255);

}