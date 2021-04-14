#include "CrossPlatform/Primitive/Primitive2D.h"
#include "CrossPlatform/Interface/IDrawerSurface.h"

#include "MathLab/MathLib.h"
#include "MathLab/Vector2f.h"
#include <cstdio>

using CrossPlatform::IDrawerSurface;
using MathLab::Vector2f;

namespace Primitive
{

    void Primitive2D::SetConfig(IDrawerSurface*const drawerSurface, IDrawerSurface*const zBuffer)
    {
        m_drawerSurface = drawerSurface;
        m_zBuffer = zBuffer;
    }

    bool Primitive2D::Shutdown()
    {
        m_drawerSurface = nullptr;
        return true;
    }

    void Primitive2D::DrawPixel(const Vector2i& pos, const SColorRGBA& rgba) const
    {
        DrawPixel(pos.x, pos.y, rgba);
    }

    void Primitive2D::DrawPixel(unsigned int x, unsigned int y, const SColorRGBA& rgba) const
    {
        m_drawerSurface->DrawPixel(x, m_drawerSurface->GetHeight() -1 - y, rgba);
        printf("(%u, %u) color: (%u, %u, %u, %u)\n", x, y, rgba.GetR(), rgba.GetG(), rgba.GetB(), rgba.GetA());
    }

    unsigned int Primitive2D::GetZbuffer(const Vector2i& pos) const
    {
        return m_zBuffer->GetPixel(pos.x, pos.y).ToRGBALittleEndian();
    }

    void Primitive2D::SetZBuffer(const Vector2i& pos, unsigned int value)
    {
        m_zBuffer->DrawPixel(pos.x, pos.y, value);
    }

    void Primitive2D::DrawLine(const Vector2i& lhs, const Vector2i& rhs, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/)const
    {
        Vector2i startPos(lhs);
        Vector2i endPos(rhs);


        int deltaX = MathLab::abs(rhs.x - lhs.x);
        int deltaY = MathLab::abs(rhs.y - lhs.y);
        bool isFlip = deltaY > deltaX? true: false;
        if (isFlip)
        {
            startPos.Swap();
            endPos.Swap();
            int tmp = deltaX;
            deltaX = deltaY;
            deltaY = tmp;
        }

        int increasementX = 1;
        int increasementY = 1;

        if (startPos.y > endPos.y)
        {

            increasementY = -1;

        }
        if (startPos.x > endPos.x)
        {

                increasementX = -1;

        }

        //error = k - 0.5
        //2error = 2k - 1
        float errorY = 2 * deltaY - deltaX;
        while (startPos.y != endPos.y)
        {
            if (isFlip)
            {
                DrawPixel(startPos.y, startPos.x, rgba);
            }
            else
            {
                DrawPixel(startPos.x, startPos.y, rgba);
            }

            startPos.x += increasementX;

            //error +=k
            //2error += 2k
            errorY += 2 * deltaY;
            if (errorY >= 0)
            {
                startPos.y += increasementY;
                errorY -= 2 * deltaX;
            }
        }
    }

    //void Primitive2D::Drawline(const Vertex2Di& lhs, const Vertex2Di&rhs, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    //{
    //    DrawLine(Vector2i(lhs.x, lhs.y), Vector2i(rhs.x, rhs.y), rgba);
    //}

    void Primitive2D::Drawline(const Vector2f& lhs, const Vector2f&rhs, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        DrawLine(Vector2i((int)lhs.x, (int)lhs.y), Vector2i((int)rhs.x, (int)rhs.y), rgba);
    }

    void Primitive2D::DrawPolygon(const Polygon2D& polygon2D) const
    {
        if (polygon2D.m_state == Polygon2D::EState::Enable)
        {
            int index;
            for (index = 0; index < polygon2D.m_numberOfVertex -1; index ++)
            {
                DrawLine(polygon2D.m_position + polygon2D.m_numberOfVertex[i],
                    polygon2D.m_position + polygon2D.m_numberOfVertex[i + !],
                    polygon2D.m_color);
            }
            DrawLine(polygon2D.m_position + polygon2D.m_numberOfVertex[polygon2D.m_numberOfVertex - 1],
                polygon2D.m_position + polygon2D.m_numberOfVertex[0],
                polygon2D.m_color);
        }
    }

    void Primitive2D::DrawTriangle(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2) const
    {
        if (p0.y == p1.y) 
        {

        }
        if (p0.y == p2.y)
        {

        }
    }

}