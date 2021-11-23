//  Mesh3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "IComponent.h"
#include "Algorithms/String.h"
#include "crossguid/guid.hpp"

namespace CrossPlatform {
	class Polygon3D;
	class Material;
}

namespace XenonEngine
{
	class Mesh3D final :public IComponent
	{
	public:
		static const float PI;

		Mesh3D(GameObject* gameobject = nullptr) :
			IComponent(gameobject) {}
		virtual ~Mesh3D()override {}
		virtual ComponentType GetComponentType() const override { return m_type; };
		virtual IComponent* Copy(GameObject*const gameObject)const override;

		virtual bool Start() override;
		virtual bool Update() override;
		virtual bool Destroy() override;

        const xg::Guid& GetModelGuid()const { return m_modelId; }
        void SetModelGuid(const xg::Guid& modelGuid);
		void LoadModel();
        const Vector< CrossPlatform::Polygon3D*>& GetPolygon3D()const { return m_polygon3D; }
		const Vector< CrossPlatform::Material*>& GetMaterials()const { return m_materials; }
        float GetMaxRadius()const { return m_maxRadius; }

        static ComponentType m_type;
	private:
        void CalculateModelMaxRadius();

        xg::Guid m_modelId;
		Vector<CrossPlatform::Polygon3D*> m_polygon3D;
		Vector<CrossPlatform::Material*> m_materials;
        float m_maxRadius = 0.0f;

	};
}