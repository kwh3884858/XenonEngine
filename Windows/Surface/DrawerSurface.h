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
        const int UNINITIALUZE_VALUE = -1;
        bool IsResolutionValid()const;
        bool IsBufferValid() const;
        bool IsPositionValid(unsigned int x, unsigned int y)const;

        SColorRGB* m_buffer;

        bool m_lock = false;
        unsigned int m_width;
        unsigned int m_height;
    };

}