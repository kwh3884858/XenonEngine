//  Primitive2D.h
//  MacWindow
//
//  Created by whkong on 2021-3-27.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2i.h"
#include "MathLab/Vector2f.h"
#include "CrossPlatform/XenonManager.h"
#include "CrossPlatform/SColorRGBA.h"
#include "CrossPlatform/Polygon2D.h"

namespace CrossPlatform {
    class IDrawerSurface;
}
using CrossPlatform::SColorRGBA;
using MathLab::Vector2i;
using MathLab::Vector2f;
using CrossPlatform::Polygon2D;

namespace Primitive {

    class Primitive2D:public CrossPlatform::XenonManager<Primitive2D>
    {
    public:

        virtual bool Initialize() override { return true; }
        void SetConfig(CrossPlatform::IDrawerSurface*const drawerSurface,
            CrossPlatform::IDrawerSurface*const zBuffer);

        virtual bool Shutdown() override ;

        void DrawPixel(const Vector2i& pos,const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawPixel(unsigned int x, unsigned int y, const SColorRGBA& rgba = CrossPlatform::WHITE) const;
        unsigned int GetZbuffer(const Vector2i& pos)const;
        void SetZBuffer(const Vector2i& pos, unsigned int value);
        void DrawLine(const Vector2i& lhs, const Vector2i& rhs, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawLine(const Vector2f& lhs, const Vector2f&rhs, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        //void DrawLine(const Vector2f* lhs, const Vector2f*rhs, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawPolygon(const Polygon2D& polygon2D)const;
        void DrawTriangle(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, const SColorRGBA& rgba = CrossPlatform::WHITE)const;

    private:
        void DrawButtomTriangle(const Vector2f& buttom, const Vector2f& p1, const Vector2f& p2, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawTopTriangle(const Vector2f& top, const Vector2f& p1, const Vector2f& p2, const SColorRGBA& rgba = CrossPlatform::WHITE)const;

        CrossPlatform::IDrawerSurface* m_drawerSurface = nullptr;
        CrossPlatform::IDrawerSurface* m_zBuffer = nullptr;

    };




}