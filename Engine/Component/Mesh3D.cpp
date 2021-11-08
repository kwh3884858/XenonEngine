#include "Mesh3D.h"
#include "Transform3D.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "Engine/Graphic/Graphic3D.h"

#include <cassert>

namespace XenonEngine
{
	IComponent* Mesh3D::Copy(GameObject*const gameObject) const
	{
		Mesh3D* that = new Mesh3D(gameObject);
		that->m_polygon3D = m_polygon3D;
        that->m_maxRadius = m_maxRadius;
		return that;
	}

	void Mesh3D::SetConfig(const Mesh3DConfig*const config)
	{
		m_polygon3D = config->m_polygon3D;
        float maxRadius = 0;
        for (int i = 0 ;i < m_polygon3D->GetNumOfVertex(); i++)
        {
            float radius = (*m_polygon3D)[i].m_vertex.x *(*m_polygon3D)[i].m_vertex.x + (*m_polygon3D)[i].m_vertex.y *(*m_polygon3D)[i].m_vertex.y + (*m_polygon3D)[i].m_vertex.z *(*m_polygon3D)[i].m_vertex.z;
            if (radius > maxRadius)
            {
                maxRadius = radius;
            }
        }
        m_maxRadius = sqrt(maxRadius);
	}

	bool Mesh3D::Start()
	{
        Graphic3D::Get().AddGameobjectToRenderList(GetGameObject());
        return true;
	}

	bool Mesh3D::Update()
	{
		//TODO

		//assert(m_polygon3D->m_numberOfVertex >= 3);
		//Transform3D* transform = GetGameObject()->GetComponent<Transform3D>();
		//assert(transform != nullptr);
		//Graphic3D::Get().RenderPolygon3D(m_polygon3D, transform);
		return true;
	}

	bool Mesh3D::Destroy()
	{
        Graphic3D::Get().RemoveGameobjectFromRenderList(GetGameObject());
		delete m_polygon3D;
		m_polygon3D = nullptr;
		return true;
	}

	ComponentType Mesh3D::m_type = ComponentType::ComponentType_Mesh3D;
}