#pragma once

#include "MathLab/Vector3f.h"
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
        Vertex2Df m_position;
        Vertex2Df m_velocity;
        SColorRGBA m_color;
        int m_numberOfVertex;
        Vertex2Df* m_vertexList;
    };
}