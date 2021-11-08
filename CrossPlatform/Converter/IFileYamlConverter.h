//  IFileYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/IFile.h"
#include "yaml-cpp/yaml.h"

namespace YAML {
    using CrossPlatform::FileHeader;

    template<>
    struct convert<FileHeader> {
        static Node encode(const FileHeader& rhs) {
            Node node;
            node.push_back(rhs.GetFileType());
            node.push_back(rhs.GetFilePath());
            node.push_back(rhs.GetGUID());
            return node;
        }

        static bool decode(const Node& node, FileHeader& rhs) {
            rhs.SetFileType(node[0].as<CrossPlatform::FileType>());
            rhs.SetFilePath(node[1].as<Algorithm::String>());
            rhs.SetGUID(node[2].as<xg::Guid>());
            return true;
        }
    };
}