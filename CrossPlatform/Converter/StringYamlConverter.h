//  StringYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "Algorithms/String.h"
#include "yaml-cpp/yaml.h"

namespace YAML {
    using Algorithm::String;
    template<>
    struct convert<String> {
        static Node encode(const String& rhs) {
            return Node(rhs.CString());
        }

        static bool decode(const Node& node, String& rhs) {
            if (!node.IsScalar())
                return false;
            rhs = node.Scalar().c_str();
            return true;
        }
    };
}