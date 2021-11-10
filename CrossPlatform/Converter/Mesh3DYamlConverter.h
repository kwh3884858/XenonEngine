#pragma once
#include "yaml-cpp/yaml.h"
#include "GUIDYamlConverter.h"
#include "Engine/Component/Mesh3D.h"

namespace YAML {
    using XenonEngine::Mesh3D;
    template<>
    struct convert<Mesh3D> {
        static Node encode(const Mesh3D& rhs) {
            Node node;
            node["ModelId"] = rhs.GetModelGuid();
            return node;
        }

        static bool decode(const Node& node, Mesh3D& rhs) {
            rhs.SetModelGuid(node["ModelId"].as<xg::Guid>());
            return true;
        }
    };
}