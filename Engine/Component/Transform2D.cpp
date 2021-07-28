#include "Transform2D.h"

namespace XenonEngine {

    Transform2D::~Transform2D()
    {
    }

    XenonEngine::IComponent* Transform2D::Copy(GameObject*const gameObject) const
    {
        Transform2D* transform = new Transform2D(gameObject);
        transform->m_isModified = m_isModified;
        transform->m_position = m_position;
        transform->m_orientation = m_orientation;
        return transform;
    }

    ComponentType Transform2D::m_type = ComponentType::ComponentType_Transform;

}