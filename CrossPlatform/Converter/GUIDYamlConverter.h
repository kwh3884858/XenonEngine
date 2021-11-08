//  GUIDYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "yaml-cpp/yaml.h"
#include "crossguid/guid.hpp"

namespace YAML {
    template<>
    struct convert<xg::Guid> {
        static Node encode(const xg::Guid& rhs) {
            return Node(rhs);
        }

        static bool decode(const Node& node, xg::Guid& rhs) {
            if (!node.IsScalar())
                return false;
            rhs = node.Scalar();
            return true;
        }
    };
}