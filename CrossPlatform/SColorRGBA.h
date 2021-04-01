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

}