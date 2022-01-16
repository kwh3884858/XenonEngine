#pragma once
#include "yaml-cpp/yaml.h"
#include "Engine/GameObject.h"
#include "Transform2DYamlConverter.h"
#include "Transform3DYamlConverter.h"
#include "Mesh3DYamlConverter.h"
#include "StringYamlConverter.h"
#include "DirectionLightYamlConverter.h"
#include "Camera3DYamlConverter.h"
#include "CameraControllerConverter.h"

namespace YAML {
    template<>
    struct convert<XenonEngine::GameObject> {
        static Node encode(const XenonEngine::GameObject& rhs) {
            Node node;
            node["Name"] = rhs.GetName();
            for (int i = 0; i < rhs.GetComponentCount(); i++)
            {
                const XenonEngine::IComponent* comp = rhs.GetComponentByIndex(i);
                switch (comp->GetComponentType())
                {
                case XenonEngine::IComponent::None:
                    break;
                case XenonEngine::IComponent::ComponentType_Transform2D:
                {
                    const XenonEngine::Transform2D& transform2D = rhs.GetComponent<XenonEngine::Transform2D>();
                    node["Transform2D"] = transform2D;
                }
                break;
                case XenonEngine::IComponent::ComponentType_Transform3D:
                {
                    const XenonEngine::Transform3D& transform3D = rhs.GetComponent<XenonEngine::Transform3D>();
                    node["Transform3D"] = transform3D;
                }
                break;
                case XenonEngine::IComponent::ComponentType_Mesh2D:
                    break;
                case XenonEngine::IComponent::ComponentType_Mesh3D:
                {
                    const XenonEngine::Mesh3D& mesh3D = rhs.GetComponent<XenonEngine::Mesh3D>();
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
                    const XenonEngine::Camera3D& camera3D = rhs.GetComponent<XenonEngine::Camera3D>();
                    node["Camera3D"] = camera3D;
                }
                    break;
                case XenonEngine::IComponent::ComponentType_Light:
                {
                    const XenonEngine::LightComponent* light = rhs.GetComponentPointer<XenonEngine::LightComponent>();
					XenonEngine::LightComponent::LightType lightType = light->GetLightType();
                    if (lightType == XenonEngine::LightComponent::Direction)
                    {
                        const XenonEngine::DirectionLightComponent& directionLight = rhs.GetComponent<XenonEngine::DirectionLightComponent>();
						XenonEngine::DirectionLightComponent lightType = rhs.GetComponent<XenonEngine::DirectionLightComponent>();
                        node["DirectionLight"] = directionLight;
                    }
                }
                    break;
				case XenonEngine::IComponent::ComponentType_CameraController:
				{
					const XenonEngine::Camera3DController& camera3D = rhs.GetComponent<XenonEngine::Camera3DController>();
					node["Camera3DController"] = camera3D;
				}
					break;
				case XenonEngine::IComponent::ComponentType_Count:
					break;
                default:
					break;

                }
            }
            return node;
        }

        static bool decode(const Node& node, XenonEngine::GameObject& rhs) {
            rhs.SetName(node["Name"].as<Algorithm::String>());
            if (node["Transform2D"].IsDefined())
            {
				XenonEngine::Transform2D* transform2d =(XenonEngine::Transform2D*) node["Transform2D"].as<XenonEngine::Transform2D>().Copy(&rhs);
				rhs.AddComponent(transform2d);
            }
            if (node["Transform3D"].IsDefined())
            {
				XenonEngine::Transform3D* transform3d =(XenonEngine::Transform3D*) node["Transform3D"].as<XenonEngine::Transform3D>().Copy(&rhs);
				rhs.AddComponent(transform3d);
            }
            if (node["Mesh3D"].IsDefined())
            {
				XenonEngine::Mesh3D* mesh3D =(XenonEngine::Mesh3D*) node["Mesh3D"].as<XenonEngine::Mesh3D>().Copy(&rhs);
                rhs.AddComponent(mesh3D);
            }
            if (node["DirectionLight"].IsDefined())
            {
				XenonEngine::DirectionLightComponent* directionLight = (XenonEngine::DirectionLightComponent*)node["DirectionLight"].as<XenonEngine::DirectionLightComponent>().Copy(&rhs);
				rhs.AddComponent(directionLight);
            }
            if (node["Camera3D"].IsDefined())
            {
				XenonEngine::Camera3D* camera = (XenonEngine::Camera3D*)node["Camera3D"].as<XenonEngine::Camera3D>().Copy(&rhs);
				rhs.AddComponent(camera);
            }
			if (node["Camera3DController"].IsDefined())
			{
				XenonEngine::Camera3DController* camera = (XenonEngine::Camera3DController*)node["Camera3DController"].as<XenonEngine::Camera3DController>().Copy(&rhs);
				rhs.AddComponent(camera);
			}

            return true;
        }
    };
}