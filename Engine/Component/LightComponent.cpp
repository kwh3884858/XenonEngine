#include "LightComponent.h"
#include "../Graphic/Graphic3D.h"

namespace XenonEngine
{

    bool LightComponent::Start()
    {
        Graphic3D::Get().AddLight(this);
        return true;
    }

    bool LightComponent::Update()
    {
        return true;
    }

    bool LightComponent::Destroy()
    {
        Graphic3D::Get().RemoveLight(this);
        return true;
    }

    ComponentType LightComponent::m_type = LightComponent::ComponentType_Light;
}