#pragma once

/////////////////Old Version//////////////////
//#include <time.h>
//
//namespace Timer {
//
//    class StoryTimer final
//    {
//    public:
//        StoryTimer() = default;
//        ~StoryTimer() = default;
//
//         void Update();
//         time_t GetTime();
//
//    private:
//        time_t m_currentTime;
//    };
//
//
//}

#include <Windows.h>
#include "CrossPlatform/Interface/ITimer.h"

namespace Timer {
    class StoryTimer final : public CrossPlatform::ITimer
    {
    public:
        StoryTimer();
        ~StoryTimer() = default;

        virtual float GetTime() const override;

    private:
        void Update();
        DWORD m_currentTime;
    };
}