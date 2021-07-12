#include <cassert>
#include "Render2D.h"
#include "Engine/Primitive/Primitive2D.h"
#include "CrossPlatform/Polygon2D.h"

namespace XenonEngine
{
    using Primitive::Primitive2D;

    Render2D::~Render2D()
    {
        delete m_polygon2D;
    }

    void Render2D::SetConfig(const Render2DConfig*const config)
    {
        m_polygon2D = config->m_polygon2D;
    }

    void Render2D::Update() const
    {
        assert(m_polygon2D->m_numberOfVertex >= 3);
        for (int i = 1; i < m_polygon2D->m_numberOfVertex - 1; i++)
        {
            Primitive2D::Get().DrawTriangle(
                m_polygon2D->m_position + m_polygon2D->m_vertexList[0],
                m_polygon2D->m_position + m_polygon2D->m_vertexList[i],
                m_polygon2D->m_position + m_polygon2D->m_vertexList[i + 1],
                m_polygon2D->m_color
            );
        }
    }

    ComponentType Render2D::m_type = ComponentType::ComponentType_Render2D;

}
