#include "PointLightComponent.h"
namespace XenonEngine
{

    XenonEngine::IComponent* PointLightComponent::Copy(GameObject*const gameObject) const
    {
        PointLightComponent* that = new PointLightComponent(gameObject);
        that->m_color = this->m_color;
        that->m_kc = this->m_kc;
        that->m_kl = this->m_kl;
        return that;
    }

    void PointLightComponent::SetConfig(const PointLightComponentConfig& config)
    {
        m_color = config.m_color;
        m_kc = config.m_kc;
        m_kl = config.m_kl;
    }

    ComponentType PointLightComponent::m_type = ComponentType::ComponentType_Light;

}