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

namespace CrossPlatform 
{
	class Polygon3D;
}
namespace XenonEngine
{
    class GameObject;
    class Camera3D;

	class Graphic3D :public CrossPlatform::XenonManager<Graphic3D>
	{
	public:
        virtual bool Initialize() override { return true; }
		virtual bool Shutdown() override { return true; }

		void AddGameobjectToRenderList( GameObject* gameobject);
		void RemoveGameobjectFromRenderList( GameObject* gameobject);
		
        void AddCamera(Camera3D* camera) { m_cameraList.Add(camera); }
        void RemoveCamera(Camera3D* camera){ m_cameraList.Remove(camera); }

        void Update()const;
	private:
        void DrawLine(const MathLab::Vector3f& start, const MathLab::Vector3f& end, const MathLab::TMatrix4X4f& localToScreenTranform, const CrossPlatform::SColorRGBA& rgba = CrossPlatform::WHITE)const;

        const Camera3D* GetMajorCamera()const;
        MathLab::TMatrix4X4f GetProjectionMatrix(const float& viewDistance, float aspectRatio)const;
        MathLab::TMatrix4X4f GetScreenMatrix(const MathLab::Vector2f& viewPort)const;
        MathLab::TMatrix4X4f GetProjectionAndScreenMatrix(const float fov, const MathLab::Vector2f& viewPort)const;

		Algorithm::Vector<GameObject*> m_renderList;
        Algorithm::Vector<Camera3D*> m_cameraList;
	};

}