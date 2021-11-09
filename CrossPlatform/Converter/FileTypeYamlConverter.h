//  StringYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/FileTypeEnum.h"
#include "yaml-cpp/yaml.h"

namespace YAML {
    using CrossPlatform::FileType;
    template<>
    struct convert<FileType> {
        static Node encode(const FileType& rhs) {
            return Node((int)rhs);
        }

        static bool decode(const Node& node, FileType& rhs) {
            rhs =(FileType) node.as<int>();
            return true;
        }
    };
}