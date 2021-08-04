#pragma once

namespace CrossPlatform {
    class ITimer
    {
    public:
		virtual ~ITimer() {};
        virtual float GetTime() const = 0;
    };
}