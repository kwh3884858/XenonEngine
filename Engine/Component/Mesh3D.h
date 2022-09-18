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

		//New
		bool IsValid()const;

		const Vertex3D operator[](int index)const;

		class iterator {
			Vertex3D num;
		public:
			iterator(Vertex3D _num = 0) : num(_num) {}
			iterator& operator++() { num = TO >= FROM ? num + 1 : num - 1; return *this; }
			iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
			bool operator==(iterator other) const { return num == other.num; }
			bool operator!=(iterator other) const { return !(*this == other); }
			Vertex3D operator*() { return num; }
			// iterator traits
			using difference_type = Vertex3D;
			using value_type = Vertex3D;
			using pointer = const Vertex3D*;
			using reference = const Vertex3D&;
			using iterator_category = std::forward_iterator_tag;
		};
		iterator begin();
		iterator end();

		//Old

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
		const Polygon3D& GetPolygon3D(const Guid& guid) const;
		Algorithm::Vector<xg::Guid> m_polygons;

		Algorithm::Vector<MathLab::Vector3f> m_vertexs;
		Algorithm::Vector<MathLab::Vector3f> m_normals;
		Algorithm::Vector<MathLab::Vector2f> m_uv;
		Algorithm::Vector<xg::Guid> m_materials;

		//Cache
		std::map<xg::Guid, const Polygon3DMeta*> m_cachePolygons;
        float m_maxRadius = 0.0f;

		bool m_requestToReload = false;
	};
}