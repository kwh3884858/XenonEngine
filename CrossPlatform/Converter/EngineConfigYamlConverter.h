//  StringYamlConverter.h
//  XenonEngine
//
//  Created by whkong on 2021-11-9.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/EngineConfig.h"
#include "CrossPlatform/Converter/StringYamlConverter.h"
#include "yaml-cpp/yaml.h"

namespace YAML {
    using CrossPlatform::EngineConfig;

    template<>
    struct convert<EngineConfig> {
        static Node encode(const EngineConfig& rhs) {
            Node node;
            node["IsFullScreen"] = rhs.m_isFullScreen;
            node["Width"] = rhs.m_width;
            node["Height"] = rhs.m_height;
            node["IsPrespectiveProjection"] = rhs.m_isPerspectiveProjection;
            node["IsEditorLaunch"] = rhs.m_isEditorLaunch;
            node["ProjectPath"] = rhs.m_projectPath;
            return node;
        }

        static bool decode(const Node& node, EngineConfig& rhs) {
            rhs.m_isFullScreen = node["IsFullScreen"].as<bool>();
            rhs.m_width = node["Width"].as<unsigned int>();
            rhs.m_height = node["Height"].as<unsigned int>();
            rhs.m_isPerspectiveProjection = node["IsPrespectiveProjection"].as<bool>();
            rhs.m_isEditorLaunch = node["IsEditorLaunch"].as<bool>();
            rhs.m_projectPath = node["ProjectPath"].as<Algorithm::String>();
            rhs.m_minX = 0;
            rhs.m_minY = 0;
            rhs.m_maxX = rhs.m_width - 1;
            rhs.m_minY = rhs.m_height - 1;;
            return true;
        }
    };
}