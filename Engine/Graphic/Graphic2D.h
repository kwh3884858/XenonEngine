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

namespace Graphic {
    struct Primitive2DConfig {
        CrossPlatform::IDrawerSurface* m_drawerSurface = nullptr;
        CrossPlatform::IDrawerSurface* m_zBuffer = nullptr;
        Vector2f m_MinDrawPosition;
        Vector2f m_MaxDrawPosition;
    };

    class Graphic2D :public CrossPlatform::XenonManager<Graphic2D>
    {
    public:
        static constexpr char Clip_Code_Cneter = 0x0000;
        static constexpr char Clip_Code_North = 0x0008;  // 1000
        static constexpr char Clip_Code_West = 0x0001;  // 0001
        static constexpr char Clip_Code_South = 0x0004;  // 0100
        static constexpr char Clip_Code_East = 0x0002;  // 0010
        static constexpr char Clip_Code_North_West = 0x0009;  // 1001
        static constexpr char Clip_Code_North_East = 0x000a;  // 1010
        static constexpr char Clip_Code_South_West = 0x0005;  // 0101
        static constexpr char Clip_Code_South_East = 0x0006;  // 0110

        virtual bool Initialize() override { return true; }
        void SetConfig(const Primitive2DConfig*const config);

        virtual bool Shutdown() override;

        void DrawPixel(const Vector2i& pos, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawPixel(unsigned int x, unsigned int y, const SColorRGBA& rgba = CrossPlatform::WHITE) const;
        unsigned int GetZbuffer(const Vector2i& pos)const;
        void SetZBuffer(const Vector2i& pos, unsigned int value);
        void DrawLine(const Vector2i& lhs, const Vector2i& rhs, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawLine(const Vector2f& lhs, const Vector2f&rhs, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        //void DrawLine(const Vector2f* lhs, const Vector2f*rhs, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawPolygon(const Polygon2D& polygon2D)const;
        void DrawTriangle(Vector2f p0, Vector2f p1, Vector2f p2, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void ClipLine(Vector2f& p0, Vector2f& p1)const;

    private:
        const int Y_AXIS_STEP = 1;

        void DrawButtomTriangle(Vector2f buttom, Vector2f p1, Vector2f p2, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawTopTriangle(Vector2f top, Vector2f p1, Vector2f p2, const SColorRGBA& rgba = CrossPlatform::WHITE)const;
        char InternalClipCode(const Vector2f& point, const Vector2f &minPosition, const Vector2f &maxPosition)const;
        bool InternalClipPoint(char clipCode,Vector2f& point, const Vector2f& anotherPoint)const;
        Vector2f InternalClipXPoint(const Vector2f& point, const Vector2f& anontherPoint, int clipX)const;
        Vector2f InternalClipYPoint(const Vector2f& point, const Vector2f& anontherPoint, int clipY)const;

        CrossPlatform::IDrawerSurface* m_drawerSurface = nullptr;
        CrossPlatform::IDrawerSurface* m_zBuffer = nullptr;
        Vector2f m_minDrawPosition;
        Vector2f m_maxDrawPosition;
    };




}