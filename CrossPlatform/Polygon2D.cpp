#include "Polygon2D.h"

namespace CrossPlatform {

    Polygon2D::Polygon2D():
        m_state(EState::Enable),
        m_position(Vector2i(0,0,0)),
        m_velocity(Vector2i(0, 0, 0)),
        m_color(SColorRGBA(255,255,255,255)),
        m_numberOfVertex(0),
        m_vertexList(nullptr)
    {
        
    }

    Polygon2D::~Polygon2D()
    {
        delete m_vertexList;
        m_vertexList = nullptr;
        m_numberOfVertex = 0;
    }

}