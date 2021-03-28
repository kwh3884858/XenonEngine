#include "CrossPlatform/Interface/IDrawerSurface.h"
namespace Primitive {

    Primitive2D::Primitive2D(IDrawerSurface* drawerSurface)
    {
        m_drawerSurface = drawerSurface;
    }



    void Primitive2D::DrawPixel(int x, int y)
    {
        m_drawerSurface->DrawPixel(x, y);
    }

    void DrawPixel(const Vector2i& pos)
    {
        m_drawerSurface->DrawPixel(pos.x, pos.y);
    }

    void DrawLine(const Vector2i& lhs, const Vector2i& rhs)
    {
        Vector2i startPos(lhs);
        Vector2i endPos(rhs);


        int deltaX = MathLab.abs(rhs.x - lhs.x);
        int deltaY = MathLab.abs(rhs.y - lhs.y);
        bool isFlip = deltaY < deltaX? true: false;
        if (isFlip)
        {
            startPos.Swap();
            endPos.Swap();
        }

        int increasementX = 1;
        int increasementY = 1;
        if (startPos.y > startPos.y)
        {
            increasementX = -1;
        }
        if (startPos.x > endPos.x)
        {
            increasementY = -1;
        }

        float errorY = 0;
        while (startPos.y != endPos.y)
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
            errorY = 2 * deltaX * errorY + 2 * deltaY;
            if (errorY > deltaX)
            {
                startPos.y += increasementY;
            }
        }
    }


}