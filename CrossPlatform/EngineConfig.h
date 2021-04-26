//  EngineConfig.h
//  MacWindow
//
//  Created by whkong on 2021-3-31.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
namespace CrossPlatform
{
    struct EngineConfig {
        bool m_isFullScreen = false;
        unsigned int m_width = 800;
        unsigned int m_height = 600;
        bool m_isPerspectiveProjection = true;

        // For draw something in the a rectangle of window
        int m_minX = 200;
        int m_minY = 150;
        int m_maxX = 600;
        int m_maxY = 450;
    };
}
