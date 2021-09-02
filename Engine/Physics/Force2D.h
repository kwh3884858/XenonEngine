#pragma once
#include "MathLab/Vector2.h"
namespace XenonPhysics
{
    using MathLab::Vector2f;
    class Force2D
    {
    public:
        Vector2f m_forceDirection;
        float fvalue = 0.0f;
    };
}

