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
	//Left hand coordinate
	class Camera3D :public IComponent
	{
	public:
		Camera3D(GameObject* gameobject) :
			IComponent(gameobject) {}
		virtual ~Camera3D()override {}
		virtual ComponentType GetComponentType() const override { return m_type; };
		virtual IComponent* Copy(GameObject*const gameObject)const override;
		MathLab::TMatrix4X3f GetCameraTransform()const;

		static ComponentType m_type;
	private:
		MathLab::Vector3f m_position;
		MathLab::Vector3f m_lookAt;
		MathLab::Vector2f m_viewDistance;
		float m_fov;
		MathLab::Vector2f m_viewport;
	};
}