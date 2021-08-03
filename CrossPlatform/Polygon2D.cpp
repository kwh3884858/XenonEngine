#include "Polygon2D.h"

namespace CrossPlatform {

    Polygon2D::Polygon2D():
        m_state(EState::Enable),
        m_color(SColorRGBA(255,255,255,255)),
        m_numberOfVertex(0),
        m_vertexList(nullptr)
    {
        
    }

    Polygon2D::Polygon2D(EState state, SColorRGBA color, int numberOfVertex, MathLab::Vector2f* vertexList) :
        m_state(state),
        m_color(color),
        m_numberOfVertex(numberOfVertex),
        m_vertexList(vertexList)
    {

    }

    Polygon2D::~Polygon2D()
    {
        delete[] m_vertexList;
        m_vertexList = nullptr;
        m_numberOfVertex = 0;
    }

}