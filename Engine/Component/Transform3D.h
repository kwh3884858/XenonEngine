//  Transform3D.h
//  XenonEngine
//
//  Created by whkong on 2021-9-1.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
#include "MathLab/Vector3.h"
#include "MathLab/Quaternion.h"
#include "MathLab/Matrix.h"

namespace XenonEngine
{
	class Rigidbody2D;

	using MathLab::Vector3f;
	using MathLab::TMatrix3X3f;
	using MathLab::TMatrix4X4f;
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

        void AddPosition(const Vector3f& position) { assert(this != nullptr); m_position += position; }
		void SetPosition(const Vector3f& position) { assert(this != nullptr); m_position = position; }
		const Vector3f& GetPosition()const { return m_position; }
        void AddRotation(const Vector3f& rotation) { assert(this != nullptr); m_orientation += rotation; }
        void SetRotation(const Vector3f& rotation) { assert(this != nullptr); m_orientation = rotation; }

		//const Quaternionf GetOrientation()const;
		const Vector3f& GetRotation() const { return m_orientation; }
        const TMatrix3X3f GetRotationTranformMatrix()const;
		const TMatrix4X4f GetLocalToWorldTransformMatrix()const;
		static ComponentType m_type;
	private:
		bool m_isModified = false;
		Vector3f m_position;
		Vector3f m_orientation;
	};
}