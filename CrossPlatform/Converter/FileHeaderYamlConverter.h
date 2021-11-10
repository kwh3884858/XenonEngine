//  IFileYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/File/IFileMeta.h"
#include "yaml-cpp/yaml.h"

#include "CrossPlatform/Converter/GUIDYamlConverter.h"
#include "CrossPlatform/Converter/StringYamlConverter.h"
#include "CrossPlatform/Converter/FileTypeYamlConverter.h"

namespace YAML {
    using CrossPlatform::FileHeader;

    template<>
    struct convert<FileHeader> {
        static Node encode(const FileHeader& rhs) {
            Node node;
            node.push_back(rhs.GetFileType());
            node.push_back(rhs.GetGUID());
            return node;
        }

        static bool decode(const Node& node, FileHeader& rhs) {
            rhs.SetFileType(node[0].as<CrossPlatform::FileType>());
            rhs.SetGUID(node[1].as<xg::Guid>());
            return true;
        }
    };
}