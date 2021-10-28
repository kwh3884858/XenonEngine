//  DirectDrawSurface.h
//  MacWindow
//
//  Created by whkong on 2021-3-22.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "CrossPlatform/Interface/IDrawerSurface.h"

#include <ddraw.h>

namespace WindowSurface {

    using CrossPlatform::IDrawerSurface;
    using CrossPlatform::SColorRGBA;

    class DirectXDrawSurface : public IDrawerSurface
    {
    public:
        DirectXDrawSurface(LPDIRECTDRAW7 lpdd7, unsigned int width, unsigned int height, int mem0ry_flags);
        virtual ~DirectXDrawSurface() override;

        virtual void lock() override;
        virtual void DrawPixel(unsigned int x, unsigned int y, SColorRGBA rgba)override;
        virtual void DrawStraightLine(unsigned int xStart, unsigned int xEnd, unsigned int y, SColorRGBA rgba)override;
        virtual SColorRGBA GetPixel(unsigned int x, unsigned int y)override;
        virtual void Unlock()override;

        LPDIRECTDRAWSURFACE7 GetDirectRawSurface() { return surface; }
        unsigned int GetWidth()const override { return m_width; }
        unsigned int GetHeight()const override { return m_height; }


    private:
        LPDIRECTDRAWSURFACE7 surface;
        //No need to release, only available when surface is locked
        DDSURFACEDESC2 m_directDrawSurfaceDescription;

        unsigned int m_width;
        unsigned int m_height;
    };

}
