//  IDrawerSurface.h
//  MacWindow
//
//  Created by whkong on 2021-3-22.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once


#include "CrossPlatform/SColorRGBA.h"
namespace CrossPlatform {
    class IDrawerSurface
    {
        virtual void lock() override;
        virtual void DrawPixel(unsigned int x, unsigned int y, SColorRGBA rgba)override;
        virtual SColorRGBA GetPixel(unsigned int x, unsigned int y)override;
        virtual void Unlock()override;
    };
}