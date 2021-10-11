#include "Mesh3D.h"
#include "Transform3D.h"
#include "CrossPlatform/Polygon3D.h"
#include "Engine/Graphic/Graphic3D.h"

#include <cassert>

namespace XenonEngine
{
	IComponent* Mesh3D::Copy(GameObject*const gameObject) const
	{
		Mesh3D* that = new Mesh3D(gameObject);
		that->m_polygon3D = m_polygon3D;
		return that;
	}

	void Mesh3D::SetConfig(const Mesh3DConfig*const config)
	{
		m_polygon3D = config->m_polygon3D;
	}

	bool Mesh3D::Start()
	{
		Graphic3D::Get().add
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
		delete m_polygon3D;
		m_polygon3D = nullptr;
		return true;
	}

	ComponentType Mesh3D::m_type = ComponentType::ComponentType_Mesh3D;
}