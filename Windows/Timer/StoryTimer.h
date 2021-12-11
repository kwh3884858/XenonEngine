#pragma once

#include "CrossPlatform/Interface/ITimer.h"
#include <chrono>
namespace Timer {

    class StoryTimer final : public CrossPlatform::ITimer
    {
    public:
        StoryTimer();
        virtual ~StoryTimer() override = default;

        void Update();
        virtual double GetTimeMilliSecond() const override;

    private:
        std::chrono::system_clock::time_point m_currentTime;
    };
}