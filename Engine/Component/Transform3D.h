//  Transform3D.h
//  XenonEngine
//
//  Created by whkong on 2021-9-1.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
#include "MathLab/Vector3.h"
#include "MathLab/Quaternion.h"

namespace XenonEngine
{
	class Rigidbody2D;

	using MathLab::Vector3f;
	using MathLab::Quaternionf;

	class Transform3D final : public IComponent
	{
	public:
		friend class Rigidbody2D;

		Transform3D(GameObject* gameobject) :
			IComponent(gameobject) {}
		virtual ~Transform3D()override;
		virtual ComponentType GetComponentType() const override { return m_type; };
		virtual IComponent* Copy(GameObject*const gameObject)const override;

		void AddPosition(const Vector3f& position) { m_position += position; }
		void SetPosition(const Vector3f& position) { m_position = position; }
		const Vector3f& GetPosition()const { return m_position; }

		const Quaternionf GetOrientation()const;
		const Vector3f& GetRotation()const { return m_orientation; }

		static ComponentType m_type;
	private:
		bool m_isModified = false;
		Vector3f m_position;
		Vector3f m_orientation;
	};
}