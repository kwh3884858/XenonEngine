#include "BoxCollider2D.h"

namespace XenonEngine
{

    BoxCollider2D::~BoxCollider2D()
    {

    }

    void BoxCollider2D::SetConfig(const BoxCollider2DConfig*const config)
    {
        mIsTrigger = config->m_isTrigger;
        m_size = config->m_size;
    }

    float BoxCollider2D::GetArea() const
    {
        return m_size.x * m_size.y;
    }

    float BoxCollider2D::GetRadius() const
    {
        return m_size.Magnitude() / 2;
    }

    XenonEngine::IComponent* BoxCollider2D::Copy(GameObject*const gameObject) const
    {
        BoxCollider2D* that = new BoxCollider2D(gameObject);
        that->m_size = m_size;
        that->mIsModified = mIsModified;
        that->mIsTrigger = mIsTrigger;
        return that;
    }

    ComponentType BoxCollider2D::m_type = ComponentType::ComponentType_Collider2D;

}