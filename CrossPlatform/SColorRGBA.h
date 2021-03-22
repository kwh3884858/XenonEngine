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

    private:
        //unsigned char GetR(int rgba);
        //unsigned char GetG(int rgba);
        //unsigned char GetB(int rgba);
        //unsigned char GetA(int rgba);

        unsigned int value;
    };
}