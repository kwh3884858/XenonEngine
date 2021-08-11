//  Polygon2D.h
//  XenonEngine
//
//  Created by whkong on 2021-4-11.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2f.h"
#include "CrossPlatform/SColorRGBA.h"
namespace CrossPlatform {

    class Polygon2D 
    {

    public:
        enum EState {
            Disable,
            Enable,
        };

        Polygon2D();
        Polygon2D(EState state, SColorRGBA color, int numberOfVertex, MathLab::Vector2f* vertexList);
        ~Polygon2D();

        EState m_state = EState::Enable;
        SColorRGBA m_color;
        int m_numberOfVertex;
        MathLab::Vector2f* m_vertexList; 
    };
}