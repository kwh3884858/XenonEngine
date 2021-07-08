#pragma once
#include "MathLab/Vector2f.h"

namespace CrossPlatform {

    class IInput
    {
    public:
        static const unsigned char RightButton = 0;
        static const unsigned char LeftButton = 1;

        virtual bool GetKeyDown(unsigned char keyCode)const = 0;
        virtual MathLab::Vector2f GetMouseMove()const = 0;
        virtual bool GetMouseButton(unsigned char mouseCode)const = 0;
    };
}