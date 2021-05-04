#include "CircleCollider2D.h"

namespace XenonEngine
{

    void CircleCollider2D::SetConfig(const CircleCollider2DConfig*const config)
    {
        m_radius = config->m_radius;
    }

}