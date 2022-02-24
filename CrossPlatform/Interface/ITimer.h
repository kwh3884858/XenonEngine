#pragma once

namespace CrossPlatform {
    class ITimer
    {
    public:
		virtual ~ITimer() {};
        virtual long GetTimeMilliSecond() const = 0;
    };
}