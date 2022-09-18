//  Mesh3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "IComponent.h"
//#include "Algorithms/String.h"
#include "MathLab/Vector3.h"
#include "Algorithms/Vector.h"
#include "crossguid/guid.hpp"
#include <map>
namespace XenonEngine
{
	class Mesh3D;
}
namespace CrossPlatform {
	class Polygon3DMeta;
	class Polygon3D;
	class MaterialMeta;
	class Vertex3D;
}
namespace YAML {
	template<>
	struct convert<XenonEngine::Mesh3D>;
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

		virtual bool Start() override { return true; }
		virtual bool Update() override { return true; }
		virtual bool Destroy() override { return true; }

		//New
		bool IsValid()const;
		int VertexCount();
		const CrossPlatform::Vertex3D operator[](int index);

		class Iterator {
			Mesh3D* m_mesh;
			int m_index;
		public:
			Iterator(Mesh3D* mesh, int index = 0) : 
				m_mesh(mesh),
				m_index(index) 
			{}
			Iterator& operator++() { m_index++; return *this; }
			Iterator operator++(int) { Iterator retval = *this; ++(*this); return retval; }
			Iterator& operator+=(int value) { m_index += value; return *this; }
			Iterator operator+(int value) { Iterator retval = *this; (*this)+=value; return retval; }
			bool operator==(Iterator other) const { return m_index == other.m_index; }
			bool operator!=(Iterator other) const { return !(*this == other); }
			CrossPlatform::Vertex3D operator*() { return (*m_mesh)[m_index]; }
			// iterator traits
			using difference_type = CrossPlatform::Vertex3D;
			using value_type = CrossPlatform::Vertex3D;
			using pointer = const CrossPlatform::Vertex3D*;
			using reference = const CrossPlatform::Vertex3D&;
			using iterator_category = std::forward_iterator_tag;
		};
		Iterator begin() { return Iterator(this, 0); }
		Iterator end() { return Iterator(this, VertexCount()); }

		//Old

        const Algorithm::Vector<xg::Guid>& GetPolygonGuids()const { return m_polygons; }
		const Algorithm::Vector<xg::Guid>& GetMaterials()const { return m_materials; }

		//const Algorithm::Vector<const CrossPlatform::Polygon3D*> GetPolygon3Ds()const;

  //      void SetModelGuid(const xg::Guid& modelGuid);
		//void LoadModel();
		//void RequestReloadModel();
        float GetMaxRadius()const { return m_maxRadius; }

        static ComponentType m_type;
	private:
        void CalculateModelMaxRadius();
		const CrossPlatform::Polygon3D& GetPolygon3D(const xg::Guid& guid) ;
		Algorithm::Vector<xg::Guid> m_polygons;

		Algorithm::Vector<MathLab::Vector3f> m_vertexs;
		Algorithm::Vector<MathLab::Vector3f> m_normals;
		Algorithm::Vector<MathLab::Vector2f> m_uv;
		Algorithm::Vector<xg::Guid> m_materials;

		//Cache
		std::map<xg::Guid, const CrossPlatform::Polygon3D*> m_cachePolygons;
        float m_maxRadius = 0.0f;

		bool m_requestToReload = false;
	};
}