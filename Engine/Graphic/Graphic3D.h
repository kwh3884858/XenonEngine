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
//#include "Engine/Graphic/RenderList3D.h"
#include "CrossPlatform/XenonShaderType.h"

namespace CrossPlatform 
{
	class Polygon3D;
    class Vertex3D;
	class Triangle3D;
}
namespace XenonEngine
{
    class GameObject;
    class Camera3D;
    class LightComponent;

 //   struct Triangle
 //   {
 //       const CrossPlatform::Vertex3D& operator[](int index)const { assert(index >= 0 && index < 3); return m_vertex[index]; }
 //       CrossPlatform::Vertex3D& operator[](int index) { return const_cast<CrossPlatform::Vertex3D&>(static_cast<const Triangle&>(*this)[index]); }
 //       CrossPlatform::Vertex3D m_vertex[3];
	//};
 //   struct TriangleIndex
 //   {
 //       int m_index;
 //       float m_zAixs;
 //   };
 //   bool IsZAxisBigger(const Triangle& lhs, const Triangle& rhs);
 //   bool IsIndexZAxisBigger(const TriangleIndex& lIndex, const TriangleIndex& rIndex);

	class Graphic3D :public CrossPlatform::XenonManager<Graphic3D>
	{
	public:
        enum CullingState : unsigned char
        {
            NotCulled,	// Inside the Culling Box 
            Culled		// Outside
        };

        enum ClippingState : unsigned char
        {
            Inside,     // Inside the view frustum
            Clipped,    // Outside
            NeedToClip, // Half inside
			NeedToClipWithAdditionalVertex
        };

		enum PlaneTestState : unsigned char
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

		struct ClipResult
		{
			ClippingState m_clippingState = ClippingState::Inside;
			CrossPlatform::Triangle3D m_additionalGeneratedTriangle;
		};

        struct VertexShaderDataInputFlat
        {
			CrossPlatform::Triangle3D m_triangle;
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
			CrossPlatform::Triangle3D m_triangle;
			//CrossPlatform::Triangle3D m_normal;
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

        //void SetRenderType(RenderType renderType) { m_renderType = renderType; }
        //const RenderType& GetRenderType()const { return m_renderType; }

        void Update()const;

		//Debug Control
		void ToggleRemoveBackFaces() { m_isRemoveBackFacesDebug = !m_isRemoveBackFacesDebug; }
		bool IsRemoveBackFaces()const { return m_isRemoveBackFacesDebug; }
	private:
        CullingState Culling(const Mesh3D& mesh, const MathLab::TMatrix4X4f& localToCameraTranform, const Camera3D& camera) const;
		//Old
        //CullingState RemoveBackFaces(const MathLab::TVector4f& p0, const MathLab::TVector4f& p1, const MathLab::TVector4f& p2) const;
		//New
        //CullingState RemoveBackFaces(const Vertex3D& p0, const Vertex3D& p1, const Vertex3D& p2)const;
        CullingState RemoveBackFaces(const CrossPlatform::Triangle3D& triangle)const;
		ClipResult Clip(CrossPlatform::Triangle3D& triagnle, const Camera3D& camera) const;

        void DrawLine(const MathLab::Vector3f& start, const MathLab::Vector3f& end, const MathLab::TMatrix4X4f& localToScreenTranform, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE) const;
        void DrawCoordinateLines(const MathLab::TMatrix4X4f& worldToScreenTranform) const;
        
        const Camera3D* GetMajorCamera() const;
        MathLab::TMatrix4X4f GetProjectionMatrix(const float& viewDistance, float aspectRatio) const;
		MathLab::TMatrix4X4f GetScreenMatrix(const MathLab::Vector2i& viewPort) const;
        MathLab::TMatrix4X4f GetProjectionAndScreenMatrix(const float fov, const MathLab::Vector2i& viewPort) const;

        void TransformLocalToCamera(CrossPlatform::Triangle3D& triangle, const MathLab::TMatrix4X4f& localToCameraTranform, const MathLab::TMatrix4X4f& worldToCameraRotationMatrix) const;

		CrossPlatform::Vertex3D Graphic3D::InternalClipZPoint(const CrossPlatform::Vertex3D& outsideVertex, const CrossPlatform::Vertex3D& insideVertex, float clipZ) const;

        //ShaderType m_renderType = ShaderType::ShaderType_Gouraud;
        Algorithm::Vector<Camera3D*> m_cameraList;
        Algorithm::Vector<LightComponent*> m_lightList;

		//RenderList3D m_renderList;

		//Debug Control
		bool m_isRemoveBackFacesDebug = true;

	};

}