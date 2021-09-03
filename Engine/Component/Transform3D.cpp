#include "Transform3D.h"

namespace XenonEngine
{
	Transform3D::~Transform3D()
	{
	}

	XenonEngine::IComponent* Transform3D::Copy(GameObject*const gameObject) const
	{
		Transform3D* transform = new Transform3D(gameObject);
		transform->m_isModified = m_isModified;
		transform->m_position = m_position;
		transform->m_orientation = m_orientation;
		return transform;
	}

	ComponentType Transform3D::m_type = ComponentType::ComponentType_Transform3D;
}