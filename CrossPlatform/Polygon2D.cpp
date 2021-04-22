#include "Polygon2D.h"

namespace CrossPlatform {

    Polygon2D::Polygon2D():
        m_state(EState::Enable),
        m_position(MathLab::Vector2f(0,0)),
        m_velocity(MathLab::Vector2f(0,0)),
        m_color(SColorRGBA(255,255,255,255)),
        m_numberOfVertex(0),
        m_vertexList(nullptr)
    {
        
    }

    Polygon2D::Polygon2D(EState state, MathLab::Vector2f position, MathLab::Vector2f velocity, SColorRGBA color, int numberOfVertex, MathLab::Vector2f* vertexList) :
        m_state(state),
        m_position(position),
        m_velocity(velocity),
        m_color(color),
        m_numberOfVertex(numberOfVertex),
        m_vertexList(vertexList)
    {

    }

    Polygon2D::~Polygon2D()
    {
        delete m_vertexList;
        m_vertexList = nullptr;
        m_numberOfVertex = 0;
    }

}