#pragma once

namespace CrossPlatform {
    class ITimer
    {
    public:
		virtual ~ITimer() {};
        virtual double GetTimeMilliSecond() const = 0;
    };
}