//  Graphic3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "CrossPlatform/XenonManager.h"
#include "Engine/Component/Transform3D.h"
#include "Algorithms/Vector.h"
#include "MathLab/Vector2.h"
#include "MathLab/Vector3.h"
#include "CrossPlatform/SColorRGBA.h"
#include "Engine/Component/Mesh3D.h"
#include "CrossPlatform/Image/Image.h"

namespace CrossPlatform 
{
	class Polygon3D;
}
namespace XenonEngine
{
    class GameObject;
    class Camera3D;
    class LightComponent;

    struct Triangle
    {
        const MathLab::TVector4f& operator[](int index)const { assert(index >= 0 && index < 3); return m_vertex[index]; }
        MathLab::TVector4f& operator[](int index) { return const_cast<MathLab::TVector4f&>(static_cast<const Triangle&>(*this)[index]); }
		MathLab::TVector4f m_vertex[3];
	};
    struct TriangleIndex
    {
        int m_index;
        float m_zAixs;
    };
    bool IsZAxisBigger(const Triangle& lhs, const Triangle& rhs);
    bool IsIndexZAxisBigger(const TriangleIndex& lIndex, const TriangleIndex& rIndex);

	class Graphic3D :public CrossPlatform::XenonManager<Graphic3D>
	{
	public:
        enum CullingState
        {
            Inside, // Culling Box Inside
            Culled  // Outside
        };

        enum ClippingState
        {
            Inside,     // Inside the view frustum
            Clipped,    // Outside
            NeedToClip  // Half inside
        };

		enum PlaneTestState
		{
			LessThanXMin,
			LessThanYMin,
			LessThanZMin,
			GreaterThanXMax,
			GreaterThanYMax,
			GreaterThanZMax,
			InsideX,
			InsideY,
			InsideZ
		};

        enum RenderType
        {
            Wireframe,
            FlatShdering,
            GouraudShdering,
        };

        struct VertexShaderDataInputFlat
        {
            Triangle m_triangle;
            CrossPlatform::SColorRGBA m_faceColor;
        };
        struct VertexShaderDataOutputFlat
        {
            MathLab::Vector2f m_screenPoint0;
            MathLab::Vector2f m_screenPoint1;
            MathLab::Vector2f m_screenPoint2;
            CrossPlatform::SColorRGBA m_faceColor;
        };
        bool VertexShaderFlat(const VertexShaderDataInputFlat& input, VertexShaderDataOutputFlat& output, const MathLab::TMatrix4X4f& worldToCameraTransform, const MathLab::TMatrix4X4f& cameraToScreenTranform) const;

        struct VertexShaderDataInputGouraud
        {
            Triangle m_vertex;
            Triangle m_normal;
            CrossPlatform::SColorRGBA m_baseColor[3];
        };
        struct VertexShaderDataOutputGouraud
        {
            MathLab::Vector2f m_screenPoint[3];
            CrossPlatform::SColorRGBA m_vertexColor[3];
        };
        bool VertexShaderGouraud(const VertexShaderDataInputGouraud& input, VertexShaderDataOutputGouraud& output, const MathLab::TMatrix4X4f& worldToCameraTransform, const MathLab::TMatrix4X4f& cameraToScreenTranform) const;
		
		struct PixelShaderDataInputGouraud
		{
			MathLab::Vector2f m_screenPoint[3];
			CrossPlatform::SColorRGBA m_vertexColor[3];
			CrossPlatform::Image m_diffuse;
		};
		struct PixelShaderDataOutputGouraud
		{

		};
		bool PixelShaderGouraud(const PixelShaderDataInputGouraud& input, PixelShaderDataOutputGouraud& output);

        virtual bool Initialize() override { return true; }
		virtual bool Shutdown() override { return true; }

        void AddCamera(Camera3D* camera) { m_cameraList.Add(camera); }
        void RemoveCamera(Camera3D* camera){ m_cameraList.Remove(camera); }

        void AddLight(LightComponent* light) { m_lightList.Add(light); }
        void RemoveLight(LightComponent* light) { m_lightList.Remove(light); }

        void SetRenderType(RenderType renderType) { m_renderType = renderType; }
        const RenderType& GetRenderType()const { return m_renderType; }

        void Update()const;
	private:
        CullingState Culling(const Mesh3D& mesh, const MathLab::TMatrix4X4f& localToCameraTranform, const Camera3D& camera) const;
		CullingState RemoveBackFaces(const MathLab::TVector4f& p0, const MathLab::TVector4f& p1, const MathLab::TVector4f& p2) const;
		ClippingState Clipping(const Triangle& triagnle, const Camera3D& camera) const;

        void DrawLine(const MathLab::Vector3f& start, const MathLab::Vector3f& end, const MathLab::TMatrix4X4f& localToScreenTranform, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE) const;
        void DrawCoordinateLines(const MathLab::TMatrix4X4f& worldToScreenTranform) const;
        
        const Camera3D* GetMajorCamera() const;
        MathLab::TMatrix4X4f GetProjectionMatrix(const float& viewDistance, float aspectRatio) const;
		MathLab::TMatrix4X4f GetScreenMatrix(const MathLab::Vector2i& viewPort) const;
        MathLab::TMatrix4X4f GetProjectionAndScreenMatrix(const float fov, const MathLab::Vector2i& viewPort) const;

        RenderType m_renderType = RenderType::GouraudShdering;
        Algorithm::Vector<Camera3D*> m_cameraList;
        Algorithm::Vector<LightComponent*> m_lightList;
	};

}