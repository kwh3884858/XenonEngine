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
        MathLab::TVector4f m_p0;
        MathLab::TVector4f m_p1;
        MathLab::TVector4f m_p2;
    };
    bool IsZAxisBigger(const Triangle& lhs, const Triangle& rhs);

	class Graphic3D :public CrossPlatform::XenonManager<Graphic3D>
	{
	public:
        enum CullingState
        {
            Inside,
            Culled
        };

        enum RenderType
        {
            Wireframe,
            FlatShdering,
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

        virtual bool Initialize() override { return true; }
		virtual bool Shutdown() override { return true; }

		void AddGameobjectToRenderList( GameObject* gameobject);
		void RemoveGameobjectFromRenderList( GameObject* gameobject);
		
        void AddCamera(Camera3D* camera) { m_cameraList.Add(camera); }
        void RemoveCamera(Camera3D* camera){ m_cameraList.Remove(camera); }

        void AddLight(LightComponent* light) { m_lightList.Add(light); }
        void RemoveLight(LightComponent* light) { m_lightList.Remove(light); }

        void Update()const;
	private:
        CullingState Culling(const Mesh3D& mesh, const MathLab::TMatrix4X4f& localToCameraTranform, const Camera3D& camera)const;
        CullingState RemoveBackFaces(const MathLab::TVector4f& p0, const MathLab::TVector4f& p1, const MathLab::TVector4f& p2)const;
        void DrawLine(const MathLab::Vector3f& start, const MathLab::Vector3f& end, const MathLab::TMatrix4X4f& localToScreenTranform, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;
        void DrawCoordinateLines(const MathLab::TMatrix4X4f& worldToScreenTranform)const;
        
        const Camera3D* GetMajorCamera()const;
        MathLab::TMatrix4X4f GetProjectionMatrix(const float& viewDistance, float aspectRatio)const;
        MathLab::TMatrix4X4f GetScreenMatrix(const MathLab::Vector2f& viewPort)const;
        MathLab::TMatrix4X4f GetProjectionAndScreenMatrix(const float fov, const MathLab::Vector2f& viewPort)const;

        RenderType m_renderType = RenderType::FlatShdering;
		Algorithm::Vector<GameObject*> m_renderList;
        Algorithm::Vector<Camera3D*> m_cameraList;
        Algorithm::Vector<LightComponent*> m_lightList;
	};

}