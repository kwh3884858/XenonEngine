#pragma once
#include "yaml-cpp/yaml.h"
#include "Vector4YamlConverter.h"
#include "Engine/Component/DirectionLightComponent.h"

namespace YAML {
    template<>
    struct convert<XenonEngine::DirectionLightComponent> {
        static Node encode(const XenonEngine::DirectionLightComponent& rhs) {
            Node node;
            node["LightType"] = (int)rhs.GetLightType();
            node["Direction"] = ((XenonEngine::DirectionLightComponent&)rhs).GetDirection();
            node["Color"] = rhs.GetRawColor();
            return node;
        }

        static bool decode(const Node& node, XenonEngine::DirectionLightComponent& rhs) {
            rhs.SetLightType((XenonEngine::LightComponent::LightType)node["LightType"].as<int>());
            rhs.SetDirection(node["Direction"].as<MathLab::Vector3f>());
            rhs.SetRawColor(node["Color"].as<MathLab::Vector4f>());
            return true;
        }
    };
}