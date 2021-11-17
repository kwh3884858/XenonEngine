#pragma once
#include "yaml-cpp/yaml.h"
#include "Vector4YamlConverter.h"
#include "Engine/Component/DirectionLightComponent.h"

namespace YAML {
    using namespace XenonEngine;
    template<>
    struct convert<DirectionLightComponent> {
        static Node encode(const DirectionLightComponent& rhs) {
            Node node;
            node["LightType"] = (int)rhs.GetLightType();
            node["Direction"] = ((DirectionLightComponent&)rhs).GetDirection();
            node["Color"] = rhs.GetRawColor();
            return node;
        }

        static bool decode(const Node& node, DirectionLightComponent& rhs) {
            rhs.SetLightType((LightComponent::LightType)node["LightType"].as<int>());
            rhs.SetDirection(node["Direction"].as<Vector3f>());
            rhs.SetRawColor(node["Color"].as<Vector4f>());
            return true;
        }
    };
}