#pragma once
#include "Algorithms/String.h"

namespace YAML {
    template<>
    struct convert<Algorithm::String> {
        static Node encode(const Algorithm::String& rhs) {
            return Node(rhs.CString());
        }

        static bool decode(const Node& node, Algorithm::String& rhs) {
            if (!node.IsScalar())
                return false;
            rhs = node.Scalar().c_str();
            return true;
        }
    };
}