//  EngineConfig.h
//  MacWindow
//
//  Created by whkong on 2021-3-31.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "Algorithms/String.h"
namespace CrossPlatform
{
    struct EngineConfig {
        bool m_isFullScreen = false;
        unsigned int m_width = 1280;
        unsigned int m_height = 720;
        bool m_isPerspectiveProjection = true;
        bool m_isEditorLaunch = true;
        Algorithm::String m_projectPath = "E:/";

        // For draw something in the a rectangle of window
        int m_minX = 0;
        int m_minY = 0;
        int m_maxX = m_width - 1;
        int m_maxY = m_height - 1;
    };
}
