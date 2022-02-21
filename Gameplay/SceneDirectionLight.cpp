#include "SceneDirectionLight.h"

#include "Engine/Component/Transform3D.h"
#include "Engine/Component/DirectionLightComponent.h"

namespace Gameplay
{
    using namespace XenonEngine;
    using namespace CrossPlatform;
    using namespace MathLab;

    SceneDirectionLight::SceneDirectionLight(const String& name) :
        GameObject(name)
    {
    }

    SceneDirectionLight::~SceneDirectionLight()
    {
    }

    SceneDirectionLight* SceneDirectionLight::Copy() const
    {
        SceneDirectionLight* that = new SceneDirectionLight("Untitled");
        return that;
    }

    void SceneDirectionLight::Start()
    {
        Transform3D* transform = new Transform3D(this);
        transform->SetPosition(Vector3f(0, 0, 3));
        AddComponent(transform);

        DirectionLightComponent* directionLightComponent = new DirectionLightComponent(this);
        DirectionLightComponentConfig config;
        config.m_color = CrossPlatform::GERY;
        config.m_direction = Vector3f(0, 0, 1);
        directionLightComponent->SetConfig(config);
        AddComponent(directionLightComponent);
    }

    void SceneDirectionLight::Update()
    {

    }

    void SceneDirectionLight::OnTrigger(GameObject* gameobject)
    {
		gameobject;
    }
}