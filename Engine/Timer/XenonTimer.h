#pragma once
#include "CrossPlatform/Interface/ITimer.h"

namespace XenonEngine {
    class XenonTimer: CrossPlatform::ITimer
    {
    public:
        XenonTimer();
        ~XenonTimer();
         float GetTime()const;
    };
}