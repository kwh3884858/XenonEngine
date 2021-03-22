//  DirectDrawSurface.h
//  MacWindow
//
//  Created by whkong on 2021-3-22.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "IDrawerSurface.h"
#include <ddraw.h>

namespace WindowSurface {

    class DirectXDrawSurface : public IDrawerSurface
    {
    public:
        DirectXDrawSurface(LPDIRECTDRAW7 lpdd7, unsigned int width, unsigned int height, int mem0ry_flags);
        ~DirectXDrawSurface();

        virtual void lock() override;
        virtual void DrawPixel(unsigned int x, unsigned int y, SColorRGBA rgba)override;
        virtual SColorRGBA GetPixel(unsigned int x, unsigned int y)override;
        virtual void Unlock()override;

    private:
        LPDIRECTDRAWSURFACE7 surface;
        DDSURFACEDESC2 m_directDrawSurfaceDescription;

        unsigned int m_width;
        unsigned int m_height;
    };

}
