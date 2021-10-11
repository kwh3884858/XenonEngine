//  Graphic3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "CrossPlatform/XenonManager.h"
#include "Engine/Component/Transform3D.h"
#include "Algorithms/Vector.h"

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
        virtual bool Initialize() override;
		virtual bool Shutdown() override { return true; }

		void AddGameobjectToRenderList( GameObject* gameobject);
		void RemoveGameobjectFromRenderList( GameObject* gameobject);
		
        void AddCamera(Camera3D* camera) { m_cameraList.Add(camera); }
        void RemoveCamera(Camera3D* camera){ m_cameraList.Remove(camera); }

        void Render()const;
	private:
        const Camera3D* GetMajorCamera()const;
        MathLab::TMatrix4X4f GetProjectileMatrix()const;

		Algorithm::Vector<GameObject*> m_renderList;
        Algorithm::Vector<Camera3D*> m_cameraList;
        MathLab::Vector2f m_viewDistance;
        float m_aspectRatio = 0.5625;
	};

}