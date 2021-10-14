//  Camera3D.h
//  XenonEngine
//
//  Created by whkong on 2021-9-3.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "MathLab/Vector3.h"
#include "MathLab/Matrix.h"
#include "IComponent.h"

namespace XenonEngine
{
    class Camera3DConfig
    {
    public:
        float m_fov;
        MathLab::Vector2f m_viewport;
    };

	//Left hand coordinate
	class Camera3D :public IComponent
	{
	public:
		Camera3D(GameObject* gameobject) :
			IComponent(gameobject) {}
		virtual ~Camera3D()override {}
		virtual ComponentType GetComponentType() const override { return m_type; };
		virtual IComponent* Copy(GameObject*const gameObject)const override;
        virtual bool Start() override;
        virtual bool Update() override;
        virtual bool Destroy() override;
        void SetConfig(const Camera3DConfig*const config);

        float GetViewDistance()const;
        float GetFov()const { return m_fov; }
        MathLab::Vector2f GetViewport()const { return m_viewport; }
        float GetAspectRatio()const { return m_viewport.x / m_viewport.y; }
        MathLab::TMatrix4X4f GetCameraTransformInverseMatrix()const;

        void SetLookAt(const MathLab::Vector3f& lookat);

		static ComponentType m_type;
	private:
        void SetEularLookAt(const MathLab::Vector2f& headingAndElevation);
		MathLab::Vector3f m_lookAt;
		float m_fov;
		MathLab::Vector2f m_viewport;
	};
}