//  Vector3YamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "yaml-cpp/yaml.h"
#include "MathLab/Vector4.h"

namespace YAML {
    using MathLab::Vector4f;
    template<>
    struct convert<Vector4f> {
        static Node encode(const Vector4f& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, Vector4f& rhs) {
            if (!node.IsSequence() || node.size() != 4) {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}