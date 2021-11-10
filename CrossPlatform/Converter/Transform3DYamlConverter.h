//  Transform3DYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "yaml-cpp/yaml.h"
#include "Vector3YamlConverter.h"
#include "Engine/Component/Transform3D.h"

namespace YAML {
    using XenonEngine::Transform3D;
    template<>
    struct convert<Transform3D> {
        static Node encode(const Transform3D& rhs) {
            Node node;
            node["Position"] = rhs.GetPosition();
            node["Rotation"] = rhs.GetRotation();
            return node;
        }

        static bool decode(const Node& node, Transform3D& rhs) {
            rhs.SetPosition(node["Position"].as<MathLab::Vector3f>());
            rhs.SetRotation(node["Rotation"].as<MathLab::Vector3f>());
            return true;
        }
    };
}