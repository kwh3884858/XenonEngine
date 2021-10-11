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
        const unsigned int m_width = 800;
        const unsigned int m_height = 600;
        const bool m_isPerspectiveProjection = true;

        // For draw something in the a rectangle of window
        const int m_minX = 0;
        const int m_minY = 0;
        const int m_maxX = 800;
        const int m_maxY = 599;
    };
}
