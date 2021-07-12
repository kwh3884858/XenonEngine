#include "CircleCollider2D.h"

namespace XenonEngine
{

    CircleCollider2D::~CircleCollider2D()
    {

    }

    void CircleCollider2D::SetConfig(const CircleCollider2DConfig*const config)
    {
        m_radius = config->m_radius;
    }

    float CircleCollider2D::GetArea() const
    {
        return m_radius * m_radius * PI;
    }

    float CircleCollider2D::GetRadius() const
    {
        return m_radius;
    }

    ComponentType CircleCollider2D::m_type = ComponentType::ComponentType_Collider2D;

    const float CircleCollider2D::PI = 3.1415926f;

}