#include "Graphic2D.h"
#include "CrossPlatform/Interface/IDrawerSurface.h"

#include "MathLab/MathLib.h"
#include "MathLab/Vector2.h"
#include <cstdio>

#include <assert.h>

using CrossPlatform::IDrawerSurface;
using MathLab::Vector2f;
//using MathLab::operator+;
//using MathLab::operator-;

namespace XenonEngine
{
    void Graphic2D::SetConfig(const Primitive2DConfig*const config)
    {
        m_drawerSurface = config->m_drawerSurface;
        m_zBuffer = config->m_zBuffer;
        m_minDrawPosition = config->m_MinDrawPosition;
        m_maxDrawPosition = config->m_MaxDrawPosition;
    }

    bool Graphic2D::Shutdown()
    {
        m_drawerSurface = nullptr;
        return true;
    }

    void Graphic2D::DrawPixel(const Vector2i& pos, const SColorRGBA& rgba) const
    {
        DrawPixel(pos.x, pos.y, rgba);
    }

    void Graphic2D::DrawPixel(unsigned int x, unsigned int y, const SColorRGBA& rgba) const
    {
        m_drawerSurface->DrawPixel(x, m_drawerSurface->GetHeight() - 1 - y, rgba);
        //printf("(%u, %u) color: (%u, %u, %u, %u)\n", x, y, rgba.GetR(), rgba.GetG(), rgba.GetB(), rgba.GetA());
    }

    unsigned int Graphic2D::GetZbuffer(const Vector2i& pos) const
    {
        return m_zBuffer->GetPixel(pos.x, pos.y).ToRGBALittleEndian();
    }

    void Graphic2D::SetZBuffer(const Vector2i& pos, unsigned int value)
    {
        m_zBuffer->DrawPixel(pos.x, pos.y, value);
    }

    void Graphic2D::DrawLine(const Vector2i& lhs, const Vector2i& rhs, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/)const
    {
        Vector2i startPos(lhs);
        Vector2i endPos(rhs);

        int deltaX = MathLab::Abs(rhs.x - lhs.x);
        int deltaY = MathLab::Abs(rhs.y - lhs.y);
        bool isFlip = deltaY > deltaX ? true : false;
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
        while (startPos.x != endPos.x)
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

    void Graphic2D::DrawLine(const Vector2f& lhs, const Vector2f&rhs, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        DrawLine(Vector2i((int)lhs.x, (int)lhs.y), Vector2i((int)rhs.x, (int)rhs.y), rgba);
    }

    void Graphic2D::DrawStraightLine(const int xStart, const int xEnd, const int y, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        m_drawerSurface->DrawStraightLine(xStart, xEnd, m_drawerSurface->GetHeight() - 1 - y, rgba);
    }

    void Graphic2D::DrawPolygon(const Polygon2D& polygon2D) const
    {
        //if (polygon2D.m_state == Polygon2D::EState::Enable)
        //{
        //    int index;
        //    for (index = 0; index < polygon2D.m_numberOfVertex - 1; index++)
        //    {
        //        DrawLine(polygon2D.m_position + polygon2D.m_vertexList[index],
        //            polygon2D.m_position + polygon2D.m_vertexList[index + 1],
        //            polygon2D.m_color);
        //    }
        //    DrawLine(polygon2D.m_position + polygon2D.m_vertexList[polygon2D.m_numberOfVertex - 1],
        //        polygon2D.m_position + polygon2D.m_vertexList[0],
        //        polygon2D.m_color);
        //}
    }

    void Graphic2D::DrawTriangle(Vector2f p0, Vector2f p1, Vector2f p2, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        if ((p0.x == p1.x && p1.x == p2.x) || (p0.y == p1.y && p1.y == p2.y))
        {
            return;
        }
        // p0 < p1 < p2 (y-axis)
        if (p0.y > p1.y)
        {
            MathLab::SwapVector(p0, p1);
        }

        if (p0.y > p2.y)
        {
            MathLab::SwapVector(p0, p2);
        }

        if (p1.y > p2.y)
        {
            MathLab::SwapVector(p1, p2);
        }

        if (p0.y == p1.y)
        {
            //verse clockwise
            if (p0.x > p1.x)
            {
                DrawTopTriangle(p2, p0, p1, rgba);
            }
            else
            {
                DrawTopTriangle(p2, p1, p0, rgba);
            }
        }
        else if (p1.y == p2.y)
        {
            if (p1.x > p2.x)
            {
                DrawButtomTriangle(p0, p2, p1, rgba);
            }
            else
            {
                DrawButtomTriangle(p0, p1, p2, rgba);
            }
        }
        else
        {
            Vector2f middlePoint = p1;
            middlePoint.x = (p1.y - p2.y) / (p0.y - p2.y) * (p0.x - p2.x) + p2.x;
            DrawTopTriangle(p2, middlePoint, p1, rgba);
            DrawButtomTriangle(p0, middlePoint, p1, rgba);
        }
    }

    Graphic2D::ClipLineState Graphic2D::ClipLine(Vector2f& p0, Vector2f& p1) const
    {
        ClipCode p0Code = InternalClipCode(p0, m_minDrawPosition, m_maxDrawPosition);
        ClipCode p1Code = InternalClipCode(p1, m_minDrawPosition, m_maxDrawPosition);
        if (p0Code & p1Code)
        {
            return ClipLineState::Eject;
        }
        if (p0Code == 0 && p1Code == 0)
        {
            return ClipLineState::Accpet;
        }
        InternalClipPoint(p0Code, p0, p1);
        InternalClipPoint(p1Code, p1, p0);
        if (p0.x >= m_minDrawPosition.x && p1.x >= m_minDrawPosition.x &&
            p0.y >= m_minDrawPosition.y && p1.y >= m_minDrawPosition.y &&
            p0.x <= m_maxDrawPosition.x && p1.x <= m_maxDrawPosition.x&&
            p0.y <= m_maxDrawPosition.y && p1.y <= m_maxDrawPosition.y)
        {
            return ClipLineState::Accpet;
        }
        else
        {
            return ClipLineState::Eject;
        }
    }

    Graphic2D::ClipLineState Graphic2D::ClipLine(Vector2i& p0, Vector2i& p1) const
    {
        Vector2f fp0(p0.x, p0.y);
        Vector2f fp1(p1.x, p1.y);
        ClipLineState state = ClipLine(fp0, fp1);
        p0 = Vector2i(fp0.x,fp0.y);
        p1 = Vector2i(fp1.x,fp1.y);

        return state;
    }

    void Graphic2D::DrawButtomTriangle(Vector2f buttom, Vector2f p1, Vector2f p2, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        //verse-clock: buttom->p1->p2
        if (p1.x >p2.x)
        {
            SwapVector(p1, p2);
        }

        Vector2f rightDelta = p2 - buttom;
        Vector2f rightIndex = buttom;
        Vector2f rightStep(0, Y_AXIS_STEP);
        rightStep.x = (rightDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(rightDelta.x / rightDelta.y);

        Vector2f leftDelta = p1 - buttom;
        Vector2f leftIndex = buttom;
        Vector2f leftStep(0, Y_AXIS_STEP);
        leftStep.x = (leftDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(leftDelta.x / leftDelta.y);

        int yButtom = MathLab::Ceil(buttom.y);
        int yTop = MathLab::Ceil(p1.y) - 1;
        if (buttom.y < m_minDrawPosition.y)
        {
            yButtom = m_minDrawPosition.y;
            leftIndex = InternalClipYPoint(buttom, p1, yButtom);
            rightIndex = InternalClipYPoint(buttom, p2, yButtom);
        }
        if (yTop > m_maxDrawPosition.y)
        {
            yTop = m_maxDrawPosition.y - 1;
        }

        if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
            p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
            buttom.x >= m_minDrawPosition.x && buttom.y <= m_maxDrawPosition.x)
        {
            while (yButtom <= yTop)
            {
                DrawStraightLine(leftIndex.x, rightIndex.x, yButtom, rgba);
                leftIndex += leftStep;
                rightIndex += rightStep;
                yButtom++;
            }
        }
        else
        {
            Vector2f left;
            Vector2f right;
            while (yButtom < yTop)
            {
                left = leftIndex;
                right = rightIndex;

                leftIndex += leftStep;
                rightIndex += rightStep;

                if (left.x < m_minDrawPosition.x)
                {
                    left.x = m_minDrawPosition.x;
                    if (right.x <= m_minDrawPosition.x)
                    {
                        continue;
                    }
                }
                if (right.x > m_maxDrawPosition.x)
                {
                    right.x = m_maxDrawPosition.x;
                    if (left.x >= m_maxDrawPosition.x)
                    {
                        continue;
                    }
                }
                DrawStraightLine(left.x, right.x, yButtom, rgba);
            }
        }
    }

    void Graphic2D::DrawTopTriangle(Vector2f top, Vector2f p1, Vector2f p2, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        //verse-clock: buttom->p1->p2
        if (p1.x < p2.x)
        {
            SwapVector(p1, p2);
        }
        Vector2f rightDelta = p1 - top;
        Vector2f rightIndex = top;
        Vector2f rightStep(0, -Y_AXIS_STEP);
        rightStep.x =( rightDelta.x > 0 ? 1.0f : -1.0f )* MathLab::Abs(rightDelta.x / rightDelta.y);

        Vector2f leftDelta = p2 - top;
        Vector2f leftIndex = top;
        Vector2f leftStep(0, -Y_AXIS_STEP);
        leftStep.x =( leftDelta.x > 0 ? 1.0f : -1.0f )* MathLab::Abs(leftDelta.x / leftDelta.y);

        if (top.y > m_maxDrawPosition.y)
        {
            rightIndex = InternalClipYPoint(top, p1, m_maxDrawPosition.y);
            leftIndex = InternalClipYPoint(top, p2, m_maxDrawPosition.y);
        }

        if (p1.y < m_minDrawPosition.y)
        {
            p1.y = m_minDrawPosition.y;
        }

        if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
            p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
            top.x >= m_minDrawPosition.x && top.x <=m_maxDrawPosition.x)
        {
            while (leftIndex.y >= p1.y)
            {
                DrawLine(leftIndex , rightIndex, rgba);
                leftIndex += leftStep;
                rightIndex += rightStep;
            }
        }
        else
        {
            Vector2f left;
            Vector2f right;
            while (leftIndex.y > p1.y)
            {
                leftIndex += leftStep;
                rightIndex += rightStep;

                left = leftIndex;
                right = rightIndex;
                if (left.x < m_minDrawPosition.x)
                {
                    left.x = m_minDrawPosition.x;
                    if (right.x <= m_minDrawPosition.x)
                    {
                        continue;
                    }
                }
                if (right.x > m_maxDrawPosition.x)
                {
                    right.x = m_maxDrawPosition.x;
                    if (left.x >= m_maxDrawPosition.x)
                    {
                        continue;
                    }
                }
                DrawLine(left , right , rgba);
            }
        }
    }

    Graphic2D::ClipCode Graphic2D::InternalClipCode(const Vector2f& point, const Vector2f &minPosition, const Vector2f &maxPosition) const
    {
        char clipCode = 0;
        if (point.x < minPosition.x)
        {
            clipCode |= Clip_Code_West;
        }
        else if (point.x > maxPosition.x)
        {
            clipCode |= Clip_Code_East;
        }

        if (point.y < minPosition.y)
        {
            clipCode |= Clip_Code_South;
        }
        else if (point.y > maxPosition.y)
        {
            clipCode |= Clip_Code_North;
        }

        return clipCode;
    }

    bool Graphic2D::InternalClipPoint(ClipCode clipCode, Vector2f& point, const Vector2f& anotherPoint) const
    {
        switch (clipCode)
        {
        case Clip_Code_North:
        {
            point = InternalClipYPoint(point, anotherPoint, m_maxDrawPosition.y);
        }
        break;
        case Clip_Code_West:
        {
            point = InternalClipXPoint(point, anotherPoint, m_minDrawPosition.x);
        }
        break;
        case Clip_Code_South:
        {
            point = InternalClipYPoint(point, anotherPoint, m_minDrawPosition.y);
        }
        break;
        case Clip_Code_East:
        {
            point = InternalClipXPoint(point, anotherPoint, m_maxDrawPosition.x);
        }
        break;
        case Clip_Code_North_West:
        {
            point = InternalClipYPoint(point, anotherPoint, m_maxDrawPosition.y);
            if (point.x < m_minDrawPosition.x || point.x > m_maxDrawPosition.x)
            {
                point = InternalClipXPoint(point, anotherPoint, m_minDrawPosition.x);
            }
        }
        break;
        case Clip_Code_North_East:
        {
            point = InternalClipYPoint(point, anotherPoint, m_maxDrawPosition.y);
            if (point.x < m_minDrawPosition.x || point.x > m_maxDrawPosition.x)
            {
                point = InternalClipXPoint(point, anotherPoint, m_maxDrawPosition.x);
            }
        }
        break;
        case Clip_Code_South_West:
        {
            point = InternalClipYPoint(point, anotherPoint, m_minDrawPosition.y);
            if (point.x < m_minDrawPosition.x || point.x > m_maxDrawPosition.x)
            {
                point = InternalClipXPoint(point, anotherPoint, m_minDrawPosition.x);
            }
        }
        break;
        case Clip_Code_South_East:
        {
            point = InternalClipYPoint(point, anotherPoint, m_minDrawPosition.y);
            if (point.x < m_minDrawPosition.x || point.x > m_maxDrawPosition.x)
            {
                point = InternalClipXPoint(point, anotherPoint, m_maxDrawPosition.x);
            }
        }
        break;
        default:
        {
            return false;
        }
        break;
        }
        return true;
    }

    Vector2f Graphic2D::InternalClipXPoint(const Vector2f& point, const Vector2f& anontherPoint, int clipX) const
    {
        Vector2f newPoint(point);
        newPoint.x = clipX;
        if ((point.x - anontherPoint.x) * (point.y - anontherPoint.y) == 0)
        {
            newPoint.y = anontherPoint.y;
        }
        else
        {
            newPoint.y = (clipX - anontherPoint.x) / (point.x - anontherPoint.x) * (point.y - anontherPoint.y) + anontherPoint.y;
        }
        return newPoint;
    }

    Vector2f Graphic2D::InternalClipYPoint(const Vector2f& point, const Vector2f& anontherPoint, int clipY) const
    {
        Vector2f newPoint(point);
        if ((point.y - clipY) * (anontherPoint.y - clipY) <= 0)
        {
            if ((point.y - anontherPoint.y) * (point.x - anontherPoint.x) == 0)
            {
                newPoint.x = anontherPoint.x;
            }
            else
            {
                newPoint.x = (clipY - anontherPoint.y) / (point.y - anontherPoint.y) * (point.x - anontherPoint.x) + anontherPoint.x;
            }
            newPoint.y = clipY;
        }

        return newPoint;
    }
}