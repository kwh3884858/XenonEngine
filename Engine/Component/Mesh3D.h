//  Mesh3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "IComponent.h"

namespace CrossPlatform {
	class Polygon3D;
}

namespace XenonEngine
{
	class Mesh3DConfig
	{
	public:
		const CrossPlatform::Polygon3D* m_polygon3D;
	};

	class Mesh3D final :public IComponent
	{
	public:
		static const float PI;

		Mesh3D(GameObject* gameobject) :
			IComponent(gameobject) {}
		virtual ~Mesh3D()override;
		virtual ComponentType GetComponentType() const override { return m_type; };
		virtual IComponent* Copy(GameObject*const gameObject)const override;

		void SetConfig(const Mesh3DConfig*const config);
		virtual bool Update() override;
		virtual bool Destroy() override;

		static ComponentType m_type;
	private:
		const CrossPlatform::Polygon3D* m_polygon3D;
	};
}