//  Polygon2D.h
//  XenonEngine
//
//  Created by whkong on 2021-4-11.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2i.h"
#include "CrossPlatform/SColorRGBA.h"
namespace CrossPlatform {
    class Vertex2Df
    {
    public:
        float x = 0;
        float y = 0;

    };

    class Vertex2Di {
    public:
        int x = 0;
        int y = 0;
    };

    class Polygon2D 
    {

    public:
        enum EState {
            Disable,
            Enable,
        };

        Polygon2D();
        ~Polygon2D();

        EState m_state = EState::Enable;
        Vector2i m_position;
        Vector2i m_velocity;
        SColorRGBA m_color;
        int m_numberOfVertex;
        Vertex2Df* m_vertexList;
    };
}