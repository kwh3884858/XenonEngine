#include "CrossPlatform/Primitive/Primitive2D.h"
#include "CrossPlatform/Interface/IDrawerSurface.h"
#include "CrossPlatform/SColorRGBA.h"
#include "MathLab/MathLib.h"

#include <cstdio>

using CrossPlatform::IDrawerSurface;
using CrossPlatform::SColorRGBA;
namespace Primitive
{

    void Primitive2D::SetConfig(IDrawerSurface*const drawerSurface, IDrawerSurface*const zBuffer)
    {
        m_drawerSurface = drawerSurface;
        m_zBuffer = zBuffer;
    }

    bool Primitive2D::shutdown()
    {
        m_drawerSurface = nullptr;
        return true;
    }

    void Primitive2D::DrawPixel(unsigned int x, unsigned int y)const
    {
        m_drawerSurface->DrawPixel(x, y, SColorRGBA(50,50,0));
        printf("(%u, %u)\n", x, y);
    }

    void Primitive2D::DrawPixel(const Vector2i& pos)const
    {
        DrawPixel(pos.x, pos.y);
    }

    unsigned int Primitive2D::GetZbuffer(const Vector2i& pos) const
    {
        m_zBuffer->GetPixel(pos.x, pos.y).ToRGBALittleEndian();
    }

    void Primitive2D::SetZBuffer(const Vector2i& pos, unsigned int value)
    {
        m_zBuffer->DrawPixel(pos.x, pos.y, value);
    }

    void Primitive2D::DrawLine(const Vector2i& lhs, const Vector2i& rhs)const
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

        float errorY = 1;
        while (startPos.y <= endPos.y)
        {
            if (isFlip)
            {
                DrawPixel(startPos.y, startPos.x);
            }
            else
            {
                DrawPixel(startPos.x, startPos.y);
            }

            startPos.x += increasementX;

            //errorY = 2 * errorY + 2 * deltaY/ deltaX > 1
            errorY += 2 * deltaY;
            if (errorY > 2* deltaX)
            {
                startPos.y += increasementY;
                errorY -= 2 * deltaX;
            }
        }
    }


}