//  Primitive2D.h
//  MacWindow
//
//  Created by whkong on 2021-3-27.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2i.h"
#include "CrossPlatform/ScaffoldSingleton.h"
#include "CrossPlatform/SColorRGBA.h"

namespace CrossPlatform {
    class IDrawerSurface;
}
using CrossPlatform::SColorRGBA;
using MathLab::Vector2i;

namespace Primitive {

    class Primitive2D:public CrossPlatform::ScaffoldSingleton<Primitive2D>
    {
    public:

        virtual bool initialize() override { return true; }
        void SetConfig(CrossPlatform::IDrawerSurface*const drawerSurface,
            CrossPlatform::IDrawerSurface*const zBuffer);

        virtual bool shutdown() override ;

        void DrawPixel(const Vector2i& pos,const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawPixel(unsigned int x, unsigned int y, const SColorRGBA& rgba = CrossPlatform::WHITE) const;
        unsigned int GetZbuffer(const Vector2i& pos)const;
        void SetZBuffer(const Vector2i& pos, unsigned int value);
        void DrawLine(const Vector2i& lhs, const Vector2i& rhs)const;

    private:
        CrossPlatform::IDrawerSurface* m_drawerSurface = nullptr;
        CrossPlatform::IDrawerSurface* m_zBuffer = nullptr;

    };




}