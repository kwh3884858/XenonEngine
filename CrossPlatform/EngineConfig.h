//  EngineConfig.h
//  MacWindow
//
//  Created by whkong on 2021-3-31.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
namespace CrossPlatform
{
    struct EngineConfig {
        const bool m_isFullScreen = false;
        const unsigned int m_width = 1280;
        const unsigned int m_height = 720;
        const bool m_isPerspectiveProjection = true;
        const bool m_isEditorLaunch = true;

        // For draw something in the a rectangle of window
        const int m_minX = 0;
        const int m_minY = 0;
        const int m_maxX = m_width - 1;
        const int m_maxY = m_height - 1;
    };
}
