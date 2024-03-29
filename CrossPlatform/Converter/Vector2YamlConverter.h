//  Vector2YamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "yaml-cpp/yaml.h"
#include "MathLab/Vector2.h"

namespace YAML {

    template<typename T>
    struct convert<MathLab::Vector2<T>> {
        static Node encode(const MathLab::Vector2<T>& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, MathLab::Vector2<T>& rhs) {
            if (!node.IsSequence() || node.size() != 2) {
                return false;
            }
            rhs.x = node[0].as<T>();
            rhs.y = node[1].as<T>();
            return true;
        }
    };
}