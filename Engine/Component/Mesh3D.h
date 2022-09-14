//  Mesh3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "IComponent.h"
//#include "Algorithms/String.h"
#include "MathLab/Vector3.h"

#include "crossguid/guid.hpp"

namespace CrossPlatform {
	class Polygon3DMeta;
	class Polygon3D;
	class MaterialMeta;
}
namespace YAML {
	template<>
	struct convert<Mesh3D>;
}
namespace XenonEngine
{
	class ObjectLoader;
	class Mesh3D final :public IComponent
	{
	public:
		friend class YAML::convert<Mesh3D>;
		friend class ObjectLoader;

		static const float PI;

		Mesh3D(GameObject* gameobject = nullptr) :
			IComponent(gameobject) {}
		Mesh3D(Mesh3D&&);
		virtual ~Mesh3D()override {}
		virtual ComponentType GetComponentType() const override { return m_type; };
		virtual IComponent* Copy(GameObject*const gameObject)const override;

		virtual bool Start() override;
		virtual bool Update() override;
		virtual bool Destroy() override;

        const Algorithm::Vector<xg::Guid>& GetPolygonGuids()const { return m_polygons; }
		const Algorithm::Vector<xg::Guid>& GetMaterials()const { return m_materials; }

		const Algorithm::Vector<const CrossPlatform::Polygon3D*> GetPolygon3Ds()const;

        void SetModelGuid(const xg::Guid& modelGuid);
		void LoadModel();
		void RequestReloadModel();
        float GetMaxRadius()const { return m_maxRadius; }

        static ComponentType m_type;
	private:
        void CalculateModelMaxRadius();

		Algorithm::Vector<xg::Guid> m_polygons;

		Algorithm::Vector<MathLab::Vector3f> m_vertexs;
		Algorithm::Vector<MathLab::Vector3f> m_normals;
		Algorithm::Vector<xg::Guid> m_materials;
        float m_maxRadius = 0.0f;

		bool m_requestToReload = false;
	};
}