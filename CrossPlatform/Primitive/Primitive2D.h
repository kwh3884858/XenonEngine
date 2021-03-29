//  Primitive2D.h
//  MacWindow
//
//  Created by whkong on 2021-3-27.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2i.h"
#include "CrossPlatform/ScaffoldSingleton.h"

namespace CrossPlatform {
    class IDrawerSurface;
}

using MathLab::Vector2i;
namespace Primitive {

    class Primitive2D:public CrossPlatform::ScaffoldSingleton<Primitive2D>
    {
    public:

        virtual bool initialize() override { return true; }
        void SetConfig(CrossPlatform::IDrawerSurface*const drawerSurface);

        virtual bool shutdown() override ;

        void DrawPixel(const Vector2i& pos)const;
        void DrawPixel(unsigned int x, unsigned int y) const;
        void DrawLine(const Vector2i& lhs, const Vector2i& rhs)const;

    private:
        CrossPlatform::IDrawerSurface* m_drawerSurface = nullptr;

    };




}