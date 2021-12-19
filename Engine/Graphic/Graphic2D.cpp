#include "Graphic2D.h"
#include "CrossPlatform/Interface/IDrawerSurface.h"

#include "MathLab/MathLib.h"
#include "MathLab/Vector2.h"
#include "CrossPlatform/Image/Image.h"
#include <cstdio>

#include <cassert>

using namespace CrossPlatform;
using namespace MathLab;

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
        if (xEnd >= xStart)
        {
            m_drawerSurface->DrawStraightLine(xStart, xEnd, m_drawerSurface->GetHeight() - 1 - y, rgba);
        }
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
                DrawBottomTriangle(p0, p2, p1, rgba);
            }
            else
            {
                DrawBottomTriangle(p0, p1, p2, rgba);
            }
        }
        else
        {
            Vector2f middlePoint = p1;
            middlePoint.x = (p0.x - p2.x) / (p0.y - p2.y) * (p1.y - p2.y) + p2.x;
            DrawTopTriangle(p2, middlePoint, p1, rgba);
            DrawBottomTriangle(p0, middlePoint, p1, rgba);
        }
    }

    void Graphic2D::DrawTriangle(const VertexData& originalData) const
    {
        if ((originalData.p0.x == originalData.p1.x && originalData.p1.x == originalData.p2.x) ||
            (originalData.p0.y == originalData.p1.y && originalData.p1.y == originalData.p2.y))
        {
            return;
        }
        Vector2f p0 (originalData.p0);
        Vector2f p1 (originalData.p1);
        Vector2f p2 (originalData.p2);

        // p0 < p1 < p2 (y-axis)
        // c0 < c1 < c2
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

        Vector4f colorTop(originalData.vcolor2);
        Vector4f colorCenter(originalData.vcolor1);
        Vector4f colorBottom(originalData.vcolor0);
        if (p2 == originalData.p1) colorTop = originalData.vcolor1;
        if (p2 == originalData.p0) colorTop = originalData.vcolor0;
        if (p1 == originalData.p0) colorCenter = originalData.vcolor0;
        if (p1 == originalData.p2) colorCenter = originalData.vcolor2;
        if (p0 == originalData.p1) colorBottom = originalData.vcolor1;
        if (p0 == originalData.p2) colorBottom = originalData.vcolor2;

        if (p0.y == p1.y)
        {
            //clockwise
            if (p1.x < p0.x )
            {
                DrawTopTriangle(p2, p0, p1, colorTop, colorBottom, colorCenter);
            }
            else
            {
                DrawTopTriangle(p2, p1, p0, colorTop, colorCenter, colorBottom);
            }
        }
        else if (p1.y == p2.y)
        {
            if (p2.x < p1.x)
            {
                DrawBottomTriangle(p0, p2, p1, colorBottom, colorTop, colorCenter);
            }
            else
            {
                DrawBottomTriangle(p0, p1, p2, colorBottom, colorCenter, colorTop);
            }
        }
        else
        {
            Vector2f middlePoint = p1;
            middlePoint.x = (p1.y - p2.y) / (p0.y - p2.y) *  (p0.x - p2.x) + p2.x;
            Vector4f middleColor = (p1.y - p2.y) / (p0.y - p2.y) * (colorBottom - colorTop) + colorTop;
            if (middlePoint.x < p1.x)
            {
                DrawTopTriangle(p2, p1, middlePoint, colorTop, colorCenter, middleColor);
                DrawBottomTriangle(p0, middlePoint, p1, colorBottom, middleColor, colorCenter);
            }
            else
            {
                DrawTopTriangle(p2, middlePoint, p1, colorTop, middleColor, colorCenter);
                DrawBottomTriangle(p0, p1, middlePoint, colorBottom, colorCenter, middleColor);
            }
        }
    }

	void Graphic2D::DrawTriangle(const VertexWithMaterialData& data) const
	{
		const VertexData& originalData = data.m_data;
		if ((originalData.p0.x == originalData.p1.x && originalData.p1.x == originalData.p2.x) ||
			(originalData.p0.y == originalData.p1.y && originalData.p1.y == originalData.p2.y))
		{
			return;
		}
		Vector2f p0(originalData.p0);
		Vector2f p1(originalData.p1);
		Vector2f p2(originalData.p2);

		// p0 < p1 < p2 (y-axis)
		// c0 < c1 < c2
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

		Vector4f colorTop(originalData.vcolor2);
		Vector4f colorCenter(originalData.vcolor1);
		Vector4f colorBottom(originalData.vcolor0);
		Vector2f uvTop(data.uv2);
		Vector2f uvCenter(data.uv1);
		Vector2f uvBottom(data.uv0);
		if (p2 == originalData.p1)
		{
			colorTop = originalData.vcolor1;
			uvTop = data.uv1;
		}
		if (p2 == originalData.p0)
		{
			colorTop = originalData.vcolor0;
			uvTop = data.uv0;
		}
		if (p1 == originalData.p0)
		{
			colorCenter = originalData.vcolor0;
			uvCenter = data.uv0;
		}
		if (p1 == originalData.p2)
		{
			colorCenter = originalData.vcolor2;
			uvCenter = data.uv2;
		}
		if (p0 == originalData.p1)
		{
			colorBottom = originalData.vcolor1;
			uvBottom = data.uv1;
		}
		if (p0 == originalData.p2)
		{
			colorBottom = originalData.vcolor2;
			uvBottom = data.uv2;
		}
		if (p0.y == p1.y)
		{
			//clockwise
			if (p1.x < p0.x)
			{
				TriangleData materialData;
				materialData.p0 = p2;
				materialData.p1 = p0;
				materialData.p2 = p1;
				materialData.vcolor0 = colorTop;
				materialData.vcolor1 = colorBottom;
				materialData.vcolor2 = colorCenter;
				materialData.uv0 = uvTop;
				materialData.uv1 = uvBottom;
				materialData.uv2 = uvCenter;
				materialData.m_diffuse = data.m_diffuse;
				DrawTopTriangle(materialData);
			}
			else
			{
				TriangleData materialData;
				materialData.p0 = p2;
				materialData.p1 = p1;
				materialData.p2 = p0;
				materialData.vcolor0 = colorTop;
				materialData.vcolor1 = colorCenter;
				materialData.vcolor2 = colorBottom;
				materialData.uv0 = uvTop;
				materialData.uv1 = uvCenter;
				materialData.uv2 = uvBottom;
				materialData.m_diffuse = data.m_diffuse;
				DrawTopTriangle(materialData);
			}
		}
		else if (p1.y == p2.y)
		{
			if (p2.x < p1.x)
			{
				TriangleData materialData;
				materialData.p0 = p0;
				materialData.p1 = p2;
				materialData.p2 = p1;
				materialData.vcolor0 = colorBottom;
				materialData.vcolor1 = colorTop;
				materialData.vcolor2 = colorCenter;
				materialData.uv0 = uvBottom;
				materialData.uv1 = uvTop;
				materialData.uv2 = uvCenter;
				materialData.m_diffuse = data.m_diffuse;
				DrawBottomTriangle(materialData);
			}
			else
			{
				TriangleData materialData;
				materialData.p0 = p0;
				materialData.p1 = p1;
				materialData.p2 = p2;
				materialData.vcolor0 = colorBottom;
				materialData.vcolor1 = colorCenter;
				materialData.vcolor2 = colorTop;
				materialData.uv0 = uvBottom;
				materialData.uv1 = uvCenter;
				materialData.uv2 = uvTop;
				materialData.m_diffuse = data.m_diffuse;
				DrawBottomTriangle(materialData);
			}
		}
		else
		{
			Vector2f middlePoint = p1;
			middlePoint.x = (p1.y - p2.y) / (p0.y - p2.y) *  (p0.x - p2.x) + p2.x;
			Vector4f middleColor = (p1.y - p2.y) / (p0.y - p2.y) * (colorBottom - colorTop) + colorTop;
			Vector2f uvMiddle = (p1.y - p2.y) / (p0.y - p2.y) * (uvBottom - uvTop) + uvTop;
			if (middlePoint.x < p1.x)
			{
				TriangleData materialDataTop;
				materialDataTop.p0 = p2;
				materialDataTop.p1 = p1;
				materialDataTop.p2 = middlePoint;
				materialDataTop.vcolor0 = colorTop;
				materialDataTop.vcolor1 = colorCenter;
				materialDataTop.vcolor2 = middleColor;
				materialDataTop.uv0 = uvTop;
				materialDataTop.uv1 = uvCenter;
				materialDataTop.uv2 = uvMiddle;
				materialDataTop.m_diffuse = data.m_diffuse;
				DrawTopTriangle(materialDataTop);

				TriangleData materialDataBottom;
				materialDataBottom.p0 = p0;
				materialDataBottom.p1 = middlePoint;
				materialDataBottom.p2 = p1;
				materialDataBottom.vcolor0 = colorBottom;
				materialDataBottom.vcolor1 = middleColor;
				materialDataBottom.vcolor2 = colorCenter;
				materialDataBottom.uv0 = uvBottom;
				materialDataBottom.uv1 = uvMiddle;
				materialDataBottom.uv2 = uvCenter;
				materialDataBottom.m_diffuse = data.m_diffuse;
				DrawBottomTriangle(p0, middlePoint, p1, colorBottom, middleColor, colorCenter);
			}
			else
			{
				TriangleData materialDataTop;
				materialDataTop.p0 = p2;
				materialDataTop.p1 = middlePoint;
				materialDataTop.p2 = p1;
				materialDataTop.vcolor0 = colorTop;
				materialDataTop.vcolor1 = middleColor;
				materialDataTop.vcolor2 = colorCenter;
				materialDataTop.uv0 = uvTop;
				materialDataTop.uv1 = uvMiddle;
				materialDataTop.uv2 = uvCenter;
				materialDataTop.m_diffuse = data.m_diffuse;
				DrawTopTriangle(materialDataTop);

				TriangleData materialDataBottom;
				materialDataBottom.p0 = p0;
				materialDataBottom.p1 = p1;
				materialDataBottom.p2 = middlePoint;
				materialDataBottom.vcolor0 = colorBottom;
				materialDataBottom.vcolor1 = colorCenter;
				materialDataBottom.vcolor2 = middleColor;
				materialDataBottom.uv0 = uvBottom;
				materialDataBottom.uv1 = uvCenter;
				materialDataBottom.uv2 = uvMiddle;
				materialDataBottom.m_diffuse = data.m_diffuse;
				DrawBottomTriangle(p0, p1, middlePoint, colorBottom, colorCenter, middleColor);
			}
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

    void Graphic2D::DrawBottomTriangle(Vector2f buttom, Vector2f p1, Vector2f p2, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
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

        int yBottom = MathLab::Ceil(buttom.y);
        int yTop = MathLab::Ceil(p1.y) - 1;
        if (buttom.y < m_minDrawPosition.y)
        {
            yBottom = m_minDrawPosition.y;
        }
        //else
        //{
        //    leftIndex.x = leftIndex.x + leftStep.x * (yButtom - buttom.y);
        //    rightIndex.x = rightIndex.x + rightStep.x * (yButtom - buttom.y);
        //}

        if (yTop > m_maxDrawPosition.y)
        {
            yTop = m_maxDrawPosition.y - 1;
        }
        leftIndex = InternalClipYPoint(buttom, p1, yBottom);
        rightIndex = InternalClipYPoint(buttom, p2, yBottom);

        if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
            p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
            buttom.x >= m_minDrawPosition.x && buttom.x <= m_maxDrawPosition.x)
        {
            while (yBottom <= yTop)
            {
                int xStart = MathLab::Ceil(leftIndex.x);
                int xEnd = MathLab::Ceil(rightIndex.x) - 1;
                DrawStraightLine(xStart, xEnd, yBottom, rgba);
                leftIndex += leftStep;
                rightIndex += rightStep;
                yBottom++;
            }
        }
        else
        {
            Vector2f left;
            Vector2f right;
            while (yBottom <= yTop)
            {
                left = leftIndex;
                right = rightIndex;

                if (left.x < m_minDrawPosition.x)
                {
                    left.x = m_minDrawPosition.x;
                    if (right.x <= m_minDrawPosition.x)
                    {
                        yBottom++;
                        continue;
                    }
                }
                if (right.x > m_maxDrawPosition.x)
                {
                    right.x = m_maxDrawPosition.x;
                    if (left.x >= m_maxDrawPosition.x)
                    {
                        yBottom++;
                        continue;
                    }
                }
                DrawStraightLine(left.x, right.x, yBottom, rgba);
                leftIndex += leftStep;
                rightIndex += rightStep;
                yBottom++;
            }
        }
    }

    void Graphic2D::DrawBottomTriangle(Vector2f bottom, Vector2f p1, Vector2f p2, Vector4f vcolorBottom, Vector4f vcolor1, Vector4f vcolor2) const
    {
        //verse-clock: bottom->p1->p2
        if (p1.x > p2.x)
        {
            SwapVector(p1, p2);
        }

        Vector2f rightDelta = p2 - bottom;
        Vector2f rightIndex = bottom;
        Vector2f rightStep(0, Y_AXIS_STEP);
        rightStep.x = (rightDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(rightDelta.x / rightDelta.y);

        Vector2f leftDelta = p1 - bottom;
        Vector2f leftIndex = bottom;
        Vector2f leftStep(0, Y_AXIS_STEP);
        leftStep.x = (leftDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(leftDelta.x / leftDelta.y);

        int yBottom = MathLab::Ceil(bottom.y);
        int yTop = MathLab::Ceil(p1.y) - 1;
        if (bottom.y < m_minDrawPosition.y)
        {
            yBottom = m_minDrawPosition.y;
        }
        if (yTop > m_maxDrawPosition.y)
        {
            yTop = m_maxDrawPosition.y - 1;
        }
        leftIndex = InternalClipYPoint(bottom, p1, yBottom);
        rightIndex = InternalClipYPoint(bottom, p2, yBottom);

        Vector4f leftColor = InternalClipColor(bottom, p1, yBottom, vcolorBottom, vcolor1);
        Vector4f RightColor = InternalClipColor(bottom, p2, yBottom, vcolorBottom, vcolor2);
        Vector4f lColorDelta = (vcolor1 - leftColor) / (yTop - yBottom);
        Vector4f rColorDelta = (vcolor2 - RightColor) / (yTop - yBottom);
        Vector4f lColorIndex = leftColor;
        Vector4f rColorIndex = RightColor;

        if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
            p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
            bottom.x >= m_minDrawPosition.x && bottom.x <= m_maxDrawPosition.x)
        {
            while (yBottom <= yTop)
            {
                int xStart = MathLab::Ceil(leftIndex.x);
                int xEnd = MathLab::Ceil(rightIndex.x) - 1;
                Vector4f strightLineDelta = (rColorIndex - lColorIndex) / (xEnd - xStart);
                Vector4f strightLineIndex = lColorIndex;
                for (; xStart <= xEnd; xStart++)
                {
                    DrawPixel(xStart, yBottom, strightLineIndex.ToColor());
                    strightLineIndex += strightLineDelta;
                }
                leftIndex += leftStep;
                rightIndex += rightStep;
                lColorIndex += lColorDelta;
                rColorIndex += rColorDelta;
                yBottom++;
            }
        }
        else
        {
            Vector2f left;
            Vector2f right;
            Vector4f strightLineDelta;
            Vector4f strightLineIndex;
            while (yBottom <= yTop)
            {
                left = leftIndex;
                right = rightIndex;
                strightLineDelta = (rColorIndex - lColorIndex) / (right.x - left.x);
                strightLineIndex = lColorIndex;
                if (left.x < m_minDrawPosition.x)
                {
                    left.x = m_minDrawPosition.x;
                    strightLineIndex += strightLineDelta * (m_minDrawPosition.x - leftIndex.x);
                    if (right.x <= m_minDrawPosition.x)
                    {
                        leftIndex += leftStep;
                        rightIndex += rightStep;
                        lColorIndex += lColorDelta;
                        rColorIndex += rColorDelta;
                        yBottom++;
                        continue;
                    }
                }
                if (right.x > m_maxDrawPosition.x)
                {
                    right.x = m_maxDrawPosition.x;
                    if (left.x >= m_maxDrawPosition.x)
                    {
                        leftIndex += leftStep;
                        rightIndex += rightStep;
                        lColorIndex += lColorDelta;
                        rColorIndex += rColorDelta;
                        yBottom++;
                        continue;
                    }
                }
                for (float i = left.x; i <= right.x; i++)
                {
                    DrawPixel(i, yBottom, strightLineIndex.ToColor());
                    strightLineIndex += strightLineDelta;
                }
                leftIndex += leftStep;
                rightIndex += rightStep;
                lColorIndex += lColorDelta;
                rColorIndex += rColorDelta;
                yBottom++;
            }
        }
    }

	void Graphic2D::DrawBottomTriangle(TriangleData& data) const
	{
        Vector2f& bottom = data.p0;
        Vector2f& p1 = data.p1;
        Vector2f p2 = data.p2;
        Vector4f& vcolorBottom = data.vcolor0;
        Vector4f& vcolor1 = data.vcolor1;
        Vector4f& vcolor2 = data.vcolor2;
		Vector2f& uvBottom = data.uv0;
		Vector2f& uv1 = data.uv1;
		Vector2f& uv2 = data.uv2;

		//verse-clock: bottom->p1->p2
		if (p1.x > p2.x)
		{
			SwapVector(p1, p2);
		}

		Vector2f rightDelta = p2 - bottom;
		Vector2f rightIndex = bottom;
		Vector2f rightStep(0, Y_AXIS_STEP);
		rightStep.x = (rightDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(rightDelta.x / rightDelta.y);

		Vector2f leftDelta = p1 - bottom;
		Vector2f leftIndex = bottom;
		Vector2f leftStep(0, Y_AXIS_STEP);
		leftStep.x = (leftDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(leftDelta.x / leftDelta.y);

		int yBottom = MathLab::Ceil(bottom.y);
		int yTop = MathLab::Ceil(p1.y) - 1;
		if (bottom.y < m_minDrawPosition.y)
		{
			yBottom = m_minDrawPosition.y;
		}
		if (yTop > m_maxDrawPosition.y)
		{
			yTop = m_maxDrawPosition.y - 1;
		}
		leftIndex = InternalClipYPoint(bottom, p1, yBottom);
		rightIndex = InternalClipYPoint(bottom, p2, yBottom);

		Vector4f leftColor = InternalClipColor(bottom, p1, yBottom, vcolorBottom, vcolor1);
		Vector4f RightColor = InternalClipColor(bottom, p2, yBottom, vcolorBottom, vcolor2);
		Vector4f lColorDelta = (vcolor1 - leftColor) / (yTop - yBottom);
		Vector4f rColorDelta = (vcolor2 - RightColor) / (yTop - yBottom);
		Vector4f lColorIndex = leftColor;
		Vector4f rColorIndex = RightColor;

		Vector2f LeftUV = InternalClipUV(bottom, p1, yBottom, uvBottom, uv1);
		Vector2f RightUV = InternalClipUV(bottom, p1, yBottom, uvBottom, uv2);
		Vector2f lUVDelta = (uv1 - LeftUV) / (yTop - yBottom);
		Vector2f rUVDelta = (uv2 - RightUV) / (yTop - yBottom);
		Vector2f lUVIndex = LeftUV;
		Vector2f rUVIndex = RightUV;
		if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
			p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
			bottom.x >= m_minDrawPosition.x && bottom.x <= m_maxDrawPosition.x)
		{
			while (yBottom <= yTop)
			{
				int xStart = MathLab::Ceil(leftIndex.x);
				int xEnd = MathLab::Ceil(rightIndex.x) - 1;
				Vector4f strightLineDelta = (rColorIndex - lColorIndex) / (xEnd - xStart);
				Vector4f strightLineIndex = lColorIndex;
				Vector2f strightLineUVDelta = (rUVIndex - lUVIndex) / (xEnd - xStart);
				Vector2f strightLineUVIndex = lUVIndex;
				for (; xStart <= xEnd; xStart++)
				{
					Vector4f samplingColor = data.m_diffuse->GetColor(strightLineUVIndex.x, strightLineUVIndex.y);
					SColorRGBA color = strightLineIndex.ToColor() * samplingColor.ToColor();
					DrawPixel(xStart, yBottom, color);
					strightLineIndex += strightLineDelta;
					strightLineUVIndex += strightLineUVDelta;
				}
				leftIndex += leftStep;
				rightIndex += rightStep;
				lColorIndex += lColorDelta;
				rColorIndex += rColorDelta;
				lUVIndex += lUVDelta;
				rUVIndex += rUVDelta;
				yBottom++;
			}
		}
		else
		{
			Vector2f left;
			Vector2f right;
			Vector4f strightLineDelta;
			Vector4f strightLineIndex;
			Vector2f strightLineUVDelta;
			Vector2f strightLineUVIndex;
			while (yBottom <= yTop)
			{
				left = leftIndex;
				right = rightIndex;
				strightLineDelta = (rColorIndex - lColorIndex) / (right.x - left.x);
				strightLineIndex = lColorIndex;
				strightLineUVDelta = (rUVIndex - lUVIndex) / (right.x - left.x);
				strightLineUVIndex = lUVIndex;
				if (left.x < m_minDrawPosition.x)
				{
					left.x = m_minDrawPosition.x;
					strightLineIndex += strightLineDelta * (m_minDrawPosition.x - leftIndex.x);
					strightLineUVIndex += strightLineUVDelta * (m_minDrawPosition.x - leftIndex.x);
					if (right.x <= m_minDrawPosition.x)
					{
						leftIndex += leftStep;
						rightIndex += rightStep;
						lColorIndex += lColorDelta;
						rColorIndex += rColorDelta;
						lUVIndex += lUVDelta;
						rUVIndex += rUVDelta;
						yBottom++;
						continue;
					}
				}
				if (right.x > m_maxDrawPosition.x)
				{
					right.x = m_maxDrawPosition.x;
					if (left.x >= m_maxDrawPosition.x)
					{
						leftIndex += leftStep;
						rightIndex += rightStep;
						lColorIndex += lColorDelta;
						rColorIndex += rColorDelta;
						lUVIndex += lUVDelta;
						rUVIndex += rUVDelta;
						yBottom++;
						continue;
					}
				}
				for (float i = left.x; i <= right.x; i++)
				{
					Vector4f samplingColor = data.m_diffuse->GetColor(strightLineUVIndex.x, strightLineUVIndex.y);
					SColorRGBA color = strightLineIndex.ToColor() * samplingColor.ToColor();
					DrawPixel(i, yBottom, color);
					strightLineIndex += strightLineDelta;
					strightLineUVIndex += strightLineUVDelta;
				}
				leftIndex += leftStep;
				rightIndex += rightStep;
				lColorIndex += lColorDelta;
				rColorIndex += rColorDelta;
				lUVIndex += lUVDelta;
				rUVIndex += rUVDelta;
				yBottom++;
			}
		}
	}

	void Graphic2D::DrawTopTriangle(Vector2f top, Vector2f p1, Vector2f p2, const SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        //verse-clock: top->p2->p1
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

        int yBottom = MathLab::Ceil(p1.y);
        int yTop = MathLab::Ceil(top.y) - 1;
        if (yBottom < m_minDrawPosition.y)
        {
            yBottom = m_minDrawPosition.y;
        }
        if (yTop > m_maxDrawPosition.y)
        {
            yTop = m_maxDrawPosition.y - 1;
        }
        rightIndex = InternalClipYPoint(top, p1, yTop);
        leftIndex = InternalClipYPoint(top, p2, yTop);
        //else
        //{
        //    leftIndex.x = leftIndex.x + leftStep.x * (yTop - top.y);
        //    rightIndex.x = rightIndex.x + rightStep.x * (yTop - top.y);
        //}

        if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
            p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
            top.x >= m_minDrawPosition.x && top.x <=m_maxDrawPosition.x)
        {
            while (yTop >= yBottom)
            {
                int xStart = MathLab::Ceil(leftIndex.x);
                int xEnd = MathLab::Ceil(rightIndex.x) - 1;
                DrawStraightLine(xStart, xEnd, yTop, rgba);
                leftIndex += leftStep;
                rightIndex += rightStep;
                yTop--;
            }
        }
        else
        {
            Vector2f left;
            Vector2f right;
            while (yTop >= yBottom)
            {
                left = leftIndex;
                right = rightIndex;
                if (left.x < m_minDrawPosition.x)
                {
                    left.x = m_minDrawPosition.x;
                    if (right.x <= m_minDrawPosition.x)
                    {
                        yTop--;
                        continue;
                    }
                }
                if (right.x > m_maxDrawPosition.x)
                {
                    right.x = m_maxDrawPosition.x;
                    if (left.x >= m_maxDrawPosition.x)
                    {
                        yTop--;
                        continue;
                    }
                }
                DrawStraightLine(left.x, right.x, yTop, rgba);
                leftIndex += leftStep;
                rightIndex += rightStep;
                yTop--;
            }
        }
    }

    void Graphic2D::DrawTopTriangle(Vector2f top, Vector2f p1, Vector2f p2, Vector4f vcolorTop, Vector4f vcolor1, Vector4f vcolor2) const
    {
        //verse-clock: buttom->p1->p2
        if (p1.x < p2.x)
        {
            SwapVector(p1, p2);
        }
        Vector2f rightDelta = p1 - top;
        Vector2f rightIndex = top;
        Vector2f rightStep(0, -Y_AXIS_STEP);
        rightStep.x = (rightDelta.x > 0 ? 1.0f : -1.0f)* MathLab::Abs(rightDelta.x / rightDelta.y);
        Vector2f leftDelta = p2 - top;
        Vector2f leftIndex = top;
        Vector2f leftStep(0, -Y_AXIS_STEP);
        leftStep.x = (leftDelta.x > 0 ? 1.0f : -1.0f)* MathLab::Abs(leftDelta.x / leftDelta.y);

        int yBottom = MathLab::Ceil(p1.y);
        int yTop = MathLab::Ceil(top.y) - 1;
        if (yBottom < m_minDrawPosition.y)
        {
            yBottom = m_minDrawPosition.y;
        }
        if (yTop > m_maxDrawPosition.y)
        {
            yTop = m_maxDrawPosition.y - 1;
        }
        rightIndex = InternalClipYPoint(top, p1, yTop);
        leftIndex = InternalClipYPoint(top, p2, yTop);

        Vector4f rightColor = InternalClipColor(top, p1, yTop, vcolorTop, vcolor1);
        Vector4f leftColor = InternalClipColor(top, p2, yTop, vcolorTop, vcolor2);
        Vector4f rColorDelta = (vcolor1 - rightColor) / (yTop - yBottom);
        Vector4f lColorDelta = (vcolor2 - leftColor) / (yTop - yBottom );
        Vector4f rColorIndex = rightColor;
        Vector4f lColorIndex = leftColor;

        if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
            p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
            top.x >= m_minDrawPosition.x && top.x <= m_maxDrawPosition.x)
        {
            while (yTop >= yBottom)
            {
                int xStart = MathLab::Ceil(leftIndex.x);
                int xEnd = MathLab::Ceil(rightIndex.x);
                Vector4f strightLineDelta = (rColorIndex - lColorIndex) / (xEnd - xStart);
                Vector4f strightLineIndex = lColorIndex;
                for (;xStart <= xEnd; xStart++)
                {
                    DrawPixel(xStart, yTop, strightLineIndex.ToColor());
                    strightLineIndex += strightLineDelta;
                }
                leftIndex += leftStep;
                rightIndex += rightStep;
                rColorIndex += rColorDelta;
                lColorIndex += lColorDelta;
                yTop--;
            }
        }
        else
        {
            Vector2f left;
            Vector2f right;
            while (yTop >= yBottom)
            {
                left = leftIndex;
                right = rightIndex;
                Vector4f strightLineDelta = (rColorIndex - lColorIndex ) / (right.x - left.x);
                Vector4f strightLineIndex = lColorIndex;
                if (left.x < m_minDrawPosition.x)
                {
                    left.x = m_minDrawPosition.x;
                    strightLineIndex += strightLineDelta * (m_minDrawPosition.x - leftIndex.x);
                    if (right.x <= m_minDrawPosition.x)
                    {
                        leftIndex += leftStep;
                        rightIndex += rightStep;
                        rColorIndex += rColorDelta;
                        lColorIndex += lColorDelta;
                        yTop--;
                        continue;
                    }
                }
                if (right.x > m_maxDrawPosition.x)
                {
                    right.x = m_maxDrawPosition.x;
                    if (left.x >= m_maxDrawPosition.x)
                    {
                        leftIndex += leftStep;
                        rightIndex += rightStep;
                        rColorIndex += rColorDelta;
                        lColorIndex += lColorDelta;
                        yTop--;
                        continue;
                    }
                }
                for (float i = left.x; i <= right.x; i++)
                {
                    DrawPixel(i, yTop, strightLineIndex.ToColor());
                    strightLineIndex += strightLineDelta;
                }
                leftIndex += leftStep;
                rightIndex += rightStep;
                rColorIndex += rColorDelta;
                lColorIndex += lColorDelta;
                yTop--;
            }
        }
    }

	void Graphic2D::DrawTopTriangle(TriangleData& data) const
	{
        Vector2f& top = data.p0;
        Vector2f& p1 = data.p1;
        Vector2f& p2 = data.p2;
        Vector4f& vcolorTop = data.vcolor0;
        Vector4f& vcolor1 = data.vcolor1;
        Vector4f& vcolor2 = data.vcolor2;
		Vector2f& uvTop = data.uv0;
		Vector2f& uv1 = data.uv1;
		Vector2f& uv2 = data.uv2;

		//verse-clock: buttom->p1->p2
		if (p1.x < p2.x)
		{
			SwapVector(p1, p2);
		}
		Vector2f rightDelta = p1 - top;
		Vector2f rightIndex = top;
		Vector2f rightStep(0, -Y_AXIS_STEP);
		rightStep.x = (rightDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(rightDelta.x / rightDelta.y);
		Vector2f leftDelta = p2 - top;
		Vector2f leftIndex = top;
		Vector2f leftStep(0, -Y_AXIS_STEP);
		leftStep.x = (leftDelta.x > 0 ? 1.0f : -1.0f) * MathLab::Abs(leftDelta.x / leftDelta.y);

		int yBottom = MathLab::Ceil(p1.y);
		int yTop = MathLab::Ceil(top.y) - 1;
		if (yBottom < m_minDrawPosition.y)
		{
			yBottom = m_minDrawPosition.y;
		}
		if (yTop > m_maxDrawPosition.y)
		{
			yTop = m_maxDrawPosition.y - 1;
		}
		rightIndex = InternalClipYPoint(top, p1, yTop);
		leftIndex = InternalClipYPoint(top, p2, yTop);

		Vector4f rightColor = InternalClipColor(top, p1, yTop, vcolorTop, vcolor1);
		Vector4f leftColor = InternalClipColor(top, p2, yTop, vcolorTop, vcolor2);
		Vector4f rColorDelta = (vcolor1 - rightColor) / (yTop - yBottom);
		Vector4f lColorDelta = (vcolor2 - leftColor) / (yTop - yBottom);
		Vector4f rColorIndex = rightColor;
		Vector4f lColorIndex = leftColor;
		Vector2f LeftUV = InternalClipUV(top, p1, yBottom, uvTop, uv1);
		Vector2f RightUV = InternalClipUV(top, p1, yBottom, uvTop, uv2);
		Vector2f lUVDelta = (uv1 - LeftUV) / (yTop - yBottom);
		Vector2f rUVDelta = (uv2 - RightUV) / (yTop - yBottom);
		Vector2f lUVIndex = LeftUV;
		Vector2f rUVIndex = RightUV;
		if (p1.x >= m_minDrawPosition.x && p1.x <= m_maxDrawPosition.x &&
			p2.x >= m_minDrawPosition.x && p2.x <= m_maxDrawPosition.x &&
			top.x >= m_minDrawPosition.x && top.x <= m_maxDrawPosition.x)
		{
			while (yTop >= yBottom)
			{
				int xStart = MathLab::Ceil(leftIndex.x);
				int xEnd = MathLab::Ceil(rightIndex.x);
				Vector4f strightLineDelta = (rColorIndex - lColorIndex) / (xEnd - xStart);
				Vector4f strightLineIndex = lColorIndex;
				Vector2f strightLineUVDelta = (rUVIndex - lUVIndex) / (xEnd - xStart);
				Vector2f strightLineUVIndex = lUVIndex;
				for (; xStart <= xEnd; xStart++)
				{
					SColorRGBA samplingColor = data.m_diffuse->GetColor(strightLineUVIndex.x, strightLineUVIndex.y);
					SColorRGBA color = strightLineIndex.ToColor() * samplingColor;
					DrawPixel(xStart, yBottom, color);
					strightLineIndex += strightLineDelta;
					strightLineUVIndex += strightLineUVDelta;
				}
				leftIndex += leftStep;
				rightIndex += rightStep;
				rColorIndex += rColorDelta;
				lColorIndex += lColorDelta;
				lUVIndex += lUVDelta;
				rUVIndex += rUVDelta;
				yTop--;
			}
		}
		else
		{
			Vector2f left;
			Vector2f right;
			Vector4f strightLineDelta;
			Vector4f strightLineIndex;
			Vector2f strightLineUVDelta;
			Vector2f strightLineUVIndex;
			while (yTop >= yBottom)
			{
				left = leftIndex;
				right = rightIndex;
				strightLineDelta = (rColorIndex - lColorIndex) / (right.x - left.x);
				strightLineIndex = lColorIndex;
				strightLineUVDelta = (rUVIndex - lUVIndex) / (right.x - left.x);
				strightLineUVIndex = lUVIndex;
				if (left.x < m_minDrawPosition.x)
				{
					left.x = m_minDrawPosition.x;
					strightLineIndex += strightLineDelta * (m_minDrawPosition.x - leftIndex.x);
					strightLineUVIndex += strightLineUVDelta * (m_minDrawPosition.x - leftIndex.x);
					if (right.x <= m_minDrawPosition.x)
					{
						leftIndex += leftStep;
						rightIndex += rightStep;
						rColorIndex += rColorDelta;
						lColorIndex += lColorDelta;
						lUVIndex += lUVDelta;
						rUVIndex += rUVDelta;
						yTop--;
						continue;
					}
				}
				if (right.x > m_maxDrawPosition.x)
				{
					right.x = m_maxDrawPosition.x;
					if (left.x >= m_maxDrawPosition.x)
					{
						leftIndex += leftStep;
						rightIndex += rightStep;
						rColorIndex += rColorDelta;
						lColorIndex += lColorDelta;
						lUVIndex += lUVDelta;
						rUVIndex += rUVDelta;
						yTop--;
						continue;
					}
				}
				for (float i = left.x; i <= right.x; i++)
				{
					Vector4f samplingColor = data.m_diffuse->GetColor(strightLineUVIndex.x, strightLineUVIndex.y);
					SColorRGBA color = strightLineIndex.ToColor() * samplingColor.ToColor();
					DrawPixel(i, yBottom, color);
					strightLineIndex += strightLineDelta;
					strightLineUVIndex += strightLineUVDelta;
				}
				leftIndex += leftStep;
				rightIndex += rightStep;
				rColorIndex += rColorDelta;
				lColorIndex += lColorDelta;
				lUVIndex += lUVDelta;
				rUVIndex += rUVDelta;
				yTop--;
			}
		}
	}

	Graphic2D::ClipCode Graphic2D::InternalClipCode(const Vector2f& point, const Vector2f& minPosition, const Vector2f& maxPosition) const
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
        //assert((point.y - clipY) * (anontherPoint.y - clipY) <= 0);
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

    Vector4f Graphic2D::InternalClipColor(const Vector2f& point, const Vector2f& anontherPoint, int clipY, const Vector4f& color, const Vector4f& anotherColor) const
    {
        Vector4f finalColor(color);
        //assert((point.y - clipY) * (anontherPoint.y - clipY) <= 0);
        if ((point.y - clipY) * (anontherPoint.y - clipY) <= 0)
        {
            if ((point.y - anontherPoint.y) * (point.x - anontherPoint.x) != 0)
            {
                finalColor = (clipY - anontherPoint.y) / (point.y - anontherPoint.y) * (color - anotherColor) + anotherColor;
            }
        }
        return finalColor;
    }

	MathLab::Vector2f Graphic2D::InternalClipUV(const MathLab::Vector2f& point, const MathLab::Vector2f& anontherPoint, int clipY, const MathLab::Vector2f& UV, const MathLab::Vector2f& anotherUV) const
	{
		Vector2f finalUV(UV);
		//assert((point.y - clipY) * (anontherPoint.y - clipY) <= 0);
		if ((point.y - clipY) * (anontherPoint.y - clipY) <= 0)
		{
			if ((point.y - anontherPoint.y) * (point.x - anontherPoint.x) != 0)
			{
				finalUV = (clipY - anontherPoint.y) / (point.y - anontherPoint.y) * (UV - anotherUV) + anotherUV;
			}
		}
		return finalUV;
	}

}