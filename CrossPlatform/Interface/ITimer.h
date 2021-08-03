#pragma once

namespace CrossPlatform {
    class ITimer
    {
    public:
        virtual float GetTime()const = 0;
    };
}