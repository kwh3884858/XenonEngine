#pragma once
#include "yaml-cpp/yaml.h"
#include "Vector2YamlConverter.h"
#include "Engine/Component/Camera3D.h"

namespace YAML {
    template<>
    struct convert<XenonEngine::Camera3D> {
        static Node encode(const XenonEngine::Camera3D& rhs) {
            Node node;
            node["Fov"] = rhs.GetFov();
            node["Viewport"] = rhs.GetViewport();
            node["NearZ"] = rhs.GetNearClipZ();
            node["FarZ"] = rhs.GetFarClipZ();
            return node;
        }

        static bool decode(const Node& node, XenonEngine::Camera3D& rhs) {
			XenonEngine::Camera3DConfig config;
            config.m_fov = node["Fov"].as<float>();
            config.m_viewport = node["Viewport"].as<MathLab::Vector2i>();
            config.m_nearClipZ = node["NearZ"].as<float>();
            config.m_farClipZ = node["FarZ"].as<float>();
            rhs.SetConfig(config);
            return true;
        }
    };
}