#pragma once
#include "yaml-cpp/yaml.h"
#include "Engine/GameObject.h"
#include "Transform2DYamlConverter.h"
#include "Transform3DYamlConverter.h"
#include "Mesh3DYamlConverter.h"
#include "StringYamlConverter.h"
#include "DirectionLightYamlConverter.h"
#include "Camera3DYamlConverter.h"

namespace YAML {
    using namespace XenonEngine;

    template<>
    struct convert<GameObject> {
        static Node encode(const GameObject& rhs) {
            Node node;
            node["Name"] = rhs.GetName();
            for (int i = 0; i < rhs.GetComponentCount(); i++)
            {
                const IComponent* comp = rhs.GetComponentByIndex(i);
                switch (comp->GetComponentType())
                {
                case XenonEngine::IComponent::None:
                    break;
                case XenonEngine::IComponent::ComponentType_Transform2D:
                {
                    const Transform2D& transform2D = rhs.GetComponent<Transform2D>();
                    node["Transform2D"] = transform2D;
                }
                break;
                case XenonEngine::IComponent::ComponentType_Transform3D:
                {
                    const Transform3D& transform3D = rhs.GetComponent<Transform3D>();
                    node["Transform3D"] = transform3D;
                }
                break;
                case XenonEngine::IComponent::ComponentType_Mesh2D:
                    break;
                case XenonEngine::IComponent::ComponentType_Mesh3D:
                {
                    const Mesh3D& mesh3D = rhs.GetComponent<Mesh3D>();
                    node["Mesh3D"] = mesh3D;
                }
                    break;
                case XenonEngine::IComponent::ComponentType_Shader:
                    break;
                case XenonEngine::IComponent::ComponentType_Collider2D:
                    break;
                case XenonEngine::IComponent::ComponentType_Rigidbody2D:
                    break;
                case XenonEngine::IComponent::ComponentType_Personality:
                    break;
                case XenonEngine::IComponent::ComponentType_Script:
                    break;
                case XenonEngine::IComponent::ComponentType_Input:
                    break;
                case XenonEngine::IComponent::ComponentType_Camera:
                {
                    const Camera3D& camera3D = rhs.GetComponent<Camera3D>();
                    node["Camera3D"] = camera3D;
                }
                    break;
                case XenonEngine::IComponent::ComponentType_Light:
                {
                    const LightComponent* light = rhs.GetComponentPointer<LightComponent>();
                    LightComponent::LightType lightType = light->GetLightType();
                    if (lightType == LightComponent::Direction)
                    {
                        const DirectionLightComponent& directionLight = rhs.GetComponent<DirectionLightComponent>();
                        DirectionLightComponent lightType = rhs.GetComponent<DirectionLightComponent>();
                        node["DirectionLight"] = directionLight;
                    }
                }
                    break;
                default:
                    break;
                }
            }
            return node;
        }

        static bool decode(const Node& node, GameObject& rhs) {
            rhs.SetName(node["Name"].as<Algorithm::String>());
            if (node["Transform2D"].IsDefined())
            {
                Transform2D* transform2d =(Transform2D*) node["Transform2D"].as<Transform2D>().Copy(&rhs);
                rhs.AddComponent(transform2d);
            }
            if (node["Transform3D"].IsDefined())
            {
                Transform3D* transform3d =(Transform3D*) node["Transform3D"].as<Transform3D>().Copy(&rhs);
                rhs.AddComponent(transform3d);
            }
            if (node["Mesh3D"].IsDefined())
            {
                Mesh3D* mesh3D =(Mesh3D*) node["Mesh3D"].as<Mesh3D>().Copy(&rhs);
                rhs.AddComponent(mesh3D);
            }
            if (node["DirectionLight"].IsDefined())
            {
                DirectionLightComponent* directionLight = (DirectionLightComponent*)node["DirectionLight"].as<DirectionLightComponent>().Copy(&rhs);
                rhs.AddComponent(directionLight);
            }

            return true;
        }
    };
}