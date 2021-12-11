//  Vector2YamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "yaml-cpp/yaml.h"
#include "MathLab/Vector2.h"

namespace YAML {

    template<>
    struct convert<MathLab::Vector2f> {
        static Node encode(const MathLab::Vector2f& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, MathLab::Vector2f& rhs) {
            if (!node.IsSequence() || node.size() != 2) {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };
}