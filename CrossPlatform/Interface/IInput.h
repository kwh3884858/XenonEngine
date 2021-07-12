#pragma once
#include "MathLab/Vector2f.h"

namespace CrossPlatform {
    const unsigned char XenonMouse_LeftButton = 0;
    const unsigned char XenonMouse_RightButton = 1;

    class IInput
    {
    public:
        virtual bool GetKeyDown(unsigned char keyCode)const = 0;
        virtual MathLab::Vector2f GetMouseMove()const = 0;
        virtual bool GetMouseButton(unsigned char mouseCode)const = 0;
        virtual MathLab::Vector2f GetAxisRaw()const = 0;
        virtual MathLab::Vector2f GetAxis()const = 0;
        virtual bool GetStickButton(unsigned buttonCode)const = 0;
    };
}