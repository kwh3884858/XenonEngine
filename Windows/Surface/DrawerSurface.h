//  DrawerSurface.h
//  MacWindow
//
//  Created by whkong on 2021-3-22.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "IDrawerSurface.h"

namespace WindowSurface {

    class DrawerSurface : public IDrawerSurface
    {
    public:
        DrawerSurface(unsigned int width, unsigned int height);
        ~DrawerSurface();

        virtual void lock() override;
        virtual void DrawPixel(unsigned int x, unsigned int y, SColorRGBA rgba)override;
        virtual SColorRGBA GetPixel(unsigned int x, unsigned int y)override;
        virtual void Unlock()override;

    private:
        SColorRGB* m_buffer;

        unsigned int m_width;
        unsigned int m_height;
    };

}