#pragma once
#include "yaml-cpp/yaml.h"
#include "Vector2YamlConverter.h"
#include "Engine/Component/Camera3D.h"

namespace YAML {
    using namespace XenonEngine;
    template<>
    struct convert<Camera3D> {
        static Node encode(const Camera3D& rhs) {
            Node node;
            node["Fov"] = rhs.GetFov();
            node["Viewport"] = rhs.GetViewport();
            node["NearZ"] = rhs.GetNearClipZ();
            node["FarZ"] = rhs.GetFarClipZ();
            return node;
        }

        static bool decode(const Node& node, Camera3D& rhs) {
            Camera3DConfig config;
            config.m_fov = node["Fov"].as<float>();
            config.m_viewport = node["Viewport"].as<Vector2f>();
            config.m_nearClipZ = node["NearZ"].as<float>();
            config.m_farClipZ = node["FarZ"].as<float>();
            rhs.SetConfig(config);
            return true;
        }
    };
}