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
		SColorRGBA(int r, int g, int b, int a = 255);
		SColorRGBA(int rgba);
		~SColorRGBA();

		unsigned int ToRGBALittleEndian();
		unsigned int ToRGBLittleEndian();

		unsigned char GetR() { return reinterpret_cast<unsigned char>(0xff & value) }
		unsigned char GetG() { return reinterpret_cast<unsigned char>(0xff & (value >> 8)) }
		unsigned char GetB() { return reinterpret_cast<unsigned char>(0xff & (value >> 16)) }
		unsigned char GetA() { return reinterpret_cast<unsigned char>(0xff & (value >> 24)) }

	private:

		unsigned int value;
	};
}