#include <cassert>
#include "Mesh2D.h"
#include "Engine/Graphic/Graphic2D.h"
#include "CrossPlatform/Polygon2D.h"

#include "Engine/GameObject.h"
#include "Engine/Component/Transform2D.h"

namespace XenonEngine
{
    using XenonEngine::Graphic2D;

	XenonEngine::IComponent* Mesh2D::Copy(GameObject*const gameObject) const
	{
		Mesh2D* that = new Mesh2D(gameObject);
		that->m_polygon2D = m_polygon2D;
		return that;
	}

    void Mesh2D::SetConfig(const Mesh2DConfig*const config)
    {
        m_polygon2D = config->m_polygon2D;
    }

	bool Mesh2D::Update()
    {
        assert(m_polygon2D->m_numberOfVertex >= 3);
        Transform2D* transform = GetGameObject()->GetComponent<Transform2D>();
        assert(transform != nullptr);
        for (int i = 1; i < m_polygon2D->m_numberOfVertex - 1; i++)
        {
            Graphic2D::Get().DrawTriangle(
                transform->GetPosition() + m_polygon2D->m_vertexList[0],
                transform->GetPosition() + m_polygon2D->m_vertexList[i],
                transform->GetPosition() + m_polygon2D->m_vertexList[i + 1],
                m_polygon2D->m_color
            );
        }
		return true;
    }

	bool Mesh2D::Destroy()
	{
		delete m_polygon2D;
		m_polygon2D = nullptr;
		return true;
	}

    ComponentType Mesh2D::m_type = ComponentType::ComponentType_Mesh2D;

}
