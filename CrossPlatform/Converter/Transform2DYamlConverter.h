//  Transform2DYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "yaml-cpp/yaml.h"
#include "Vector2YamlConverter.h"
#include "Engine/Component/Transform2D.h"

namespace YAML {
    using XenonEngine::Transform2D;
    template<>
    struct convert<Transform2D> {
        static Node encode(const Transform2D& rhs) {
            Node node;
            node["Position"] = rhs.GetPosition();
            node["Orientation"] = rhs.GetOrientation();
            return node;
        }

        static bool decode(const Node& node, Transform2D& rhs) {
            rhs.SetPosition(node["Position"].as<MathLab::Vector2f>());
            rhs.SetOrientation(node["Orientation"].as<float>());
            return true;
        }
    };
}