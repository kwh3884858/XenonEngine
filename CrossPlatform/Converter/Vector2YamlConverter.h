#pragma once
#include "yaml-cpp/yaml.h"
#include "MathLab/Vector2.h"

namespace YAML {
    using MathLab::Vector2f;

    template<>
    struct convert<Vector2f> {
        static Node encode(const Vector2f& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, Vector2f& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }
            rhs.x = node[0].as<double>();
            rhs.y = node[1].as<double>();
            return true;
        }
    };
}