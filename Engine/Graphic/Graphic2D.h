//  Primitive2D.h
//  MacWindow
//
//  Created by whkong on 2021-3-27.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2.h"
#include "MathLab/Vector4.h"
#include "CrossPlatform/XenonManager.h"
#include "CrossPlatform/SColorRGBA.h"
#include "CrossPlatform/Polygon/Polygon2D.h"

namespace CrossPlatform {
    class IDrawerSurface;
	class Image;
}

namespace XenonEngine {
    struct Primitive2DConfig {
        CrossPlatform::IDrawerSurface* m_drawerSurface = nullptr;
        CrossPlatform::IDrawerSurface* m_zBuffer = nullptr;
        MathLab::Vector2i m_MinDrawPosition;
        MathLab::Vector2i m_MaxDrawPosition;
    };

    struct VertexData
    {
        MathLab::Vector2f p0;
        MathLab::Vector2f p1;
        MathLab::Vector2f p2;
        MathLab::Vector4f vcolor0;
        MathLab::Vector4f vcolor1;
        MathLab::Vector4f vcolor2;
    };

	struct VertexWithMaterialData
	{
		VertexData m_data;
		MathLab::Vector2f uv0;
		MathLab::Vector2f uv1;
		MathLab::Vector2f uv2;
		CrossPlatform::Image* m_diffuse;
	};

    class Graphic2D :public CrossPlatform::XenonManager<Graphic2D>
    {
    public:
        typedef char ClipCode;
        static constexpr ClipCode Clip_Code_Cneter = 0x0000;
        static constexpr ClipCode Clip_Code_North = 0x0008;  // 1000
        static constexpr ClipCode Clip_Code_West = 0x0001;  // 0001
        static constexpr ClipCode Clip_Code_South = 0x0004;  // 0100
        static constexpr ClipCode Clip_Code_East = 0x0002;  // 0010
        static constexpr ClipCode Clip_Code_North_West = 0x0009;  // 1001
        static constexpr ClipCode Clip_Code_North_East = 0x000a;  // 1010
        static constexpr ClipCode Clip_Code_South_West = 0x0005;  // 0101
        static constexpr ClipCode Clip_Code_South_East = 0x0006;  // 0110

        enum ClipLineState
        {
            Accpet,
            Eject
        };

        virtual bool Initialize() override { return true; }
        void SetConfig(const Primitive2DConfig*const config);

        virtual bool Shutdown() override;

        void DrawPixel(const MathLab::Vector2i& pos, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawPixel(unsigned int x, unsigned int y, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE) const;
        unsigned int GetZbuffer(const MathLab::Vector2i& pos)const;
        void SetZBuffer(const MathLab::Vector2i& pos, unsigned int value);
        void DrawLine(const MathLab::Vector2i& lhs, const MathLab::Vector2i& rhs, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawLine(const MathLab::Vector2f& lhs, const MathLab::Vector2f&rhs, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawStraightLine(const int xStart, const int xEnd, const int y, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        //void DrawLine(const MathLab::Vector2f* lhs, const MathLab::Vector2f*rhs, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        //void DrawPolygon(const CrossPlatform::Polygon2D& polygon2D)const;
        void DrawTriangle(MathLab::Vector2f p0, MathLab::Vector2f p1, MathLab::Vector2f p2, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawTriangle(const VertexData& originalData)const;
		void DrawTriangle(const VertexWithMaterialData& data)const;
        ClipLineState ClipLine(MathLab::Vector2f& p0, MathLab::Vector2f& p1)const;
        ClipLineState ClipLine(MathLab::Vector2i& p0, MathLab::Vector2i& p1)const;

    private:
        const int Y_AXIS_STEP = 1;

        void DrawBottomTriangle(MathLab::Vector2f buttom, MathLab::Vector2f p1, MathLab::Vector2f p2, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawTopTriangle(MathLab::Vector2f top, MathLab::Vector2f p1, MathLab::Vector2f p2, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawBottomTriangle(MathLab::Vector2f buttom, MathLab::Vector2f p1, MathLab::Vector2f p2, 
            MathLab::Vector4f vcolorButtom, MathLab::Vector4f vcolor1, MathLab::Vector4f vcolor2)const;
        void DrawTopTriangle(MathLab::Vector2f top, MathLab::Vector2f p1, MathLab::Vector2f p2,
            MathLab::Vector4f vcolorTop, MathLab::Vector4f vcolor1, MathLab::Vector4f vcolor2)const;

		struct TriangleData
		{
			MathLab::Vector2f p0;
			MathLab::Vector2f p1;
			MathLab::Vector2f p2;
			MathLab::Vector4f vcolor0;
			MathLab::Vector4f vcolor1;
			MathLab::Vector4f vcolor2;
			MathLab::Vector2f uv0;
			MathLab::Vector2f uv1;
			MathLab::Vector2f uv2;
			CrossPlatform::Image* m_diffuse;
		};
		void DrawBottomTriangle( TriangleData& data)const;
		void DrawTopTriangle( TriangleData& data)const;

        ClipCode InternalClipCode(const MathLab::Vector2f& point, const MathLab::Vector2i &minPosition, const MathLab::Vector2i &maxPosition)const;
        bool InternalClipPoint(ClipCode clipCode,MathLab::Vector2f& point, const MathLab::Vector2f& anotherPoint)const;
        MathLab::Vector2f InternalClipXPoint(const MathLab::Vector2f& point, const MathLab::Vector2f& anontherPoint, int clipX)const;
        MathLab::Vector2f InternalClipYPoint(const MathLab::Vector2f& point, const MathLab::Vector2f& anontherPoint, int clipY)const;
		MathLab::Vector4f InternalClipColor(const MathLab::Vector2f& point, const MathLab::Vector2f& anontherPoint, int clipY, const MathLab::Vector4f& color, const MathLab::Vector4f& anotherColor) const;
		MathLab::Vector2f InternalClipUV(const MathLab::Vector2f& point, const MathLab::Vector2f& anontherPoint, int clipY, const MathLab::Vector2f& UV, const MathLab::Vector2f& anotherUV) const;

        CrossPlatform::IDrawerSurface* m_drawerSurface = nullptr;
        CrossPlatform::IDrawerSurface* m_zBuffer = nullptr;
        MathLab::Vector2i m_minDrawPosition;
        MathLab::Vector2i m_maxDrawPosition;
    };




}