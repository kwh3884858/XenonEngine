//  Primitive2D.h
//  MacWindow
//
//  Created by whkong on 2021-3-27.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2i.h"

#endif // 
namespace Primitive {

    class Primitive2D
    {
    public:
        Primitive2D(IDrawerSurface* drawerSurface);

        void DrawPixel(const Vector2i& pos);
        void DrawPixel(int x, int y);
        void DrawLine(const Vector2i& lhs, const Vector2i& rhs);

    private:
        IDrawerSurface* m_drawerSurface;

    };




}