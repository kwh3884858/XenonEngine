#include "DirectionLightComponent.h"
namespace XenonEngine
{

    XenonEngine::IComponent* DirectionLightComponent::Copy(GameObject*const gameObject) const
    {
        DirectionLightComponent* that = new DirectionLightComponent(gameObject);
        that->m_direction = this->m_direction;
        that->m_color = this->m_color;
        return that;
    }

    void DirectionLightComponent::SetConfig(const DirectionLightComponentConfig& config)
    {
        m_direction = config.m_direction;
        m_color = config.m_color;

    }

    MathLab::Vector3f DirectionLightComponent::GetDirection()
    {
        if (!m_direction.IsNormalized())
        {
            m_direction.Normalized();
        }
        return m_direction;
    }

    ComponentType DirectionLightComponent::m_type = ComponentType::ComponentType_Light;

}