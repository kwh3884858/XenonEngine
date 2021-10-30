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

//#include <Windows.h>
#include "CrossPlatform/Interface/ITimer.h"
#include <chrono>
namespace Timer {

    using namespace std;
    using namespace std::chrono;

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