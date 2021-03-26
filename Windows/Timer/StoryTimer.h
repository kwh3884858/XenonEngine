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

namespace Timer {

    class StoryTimer final
    {
    public:
        StoryTimer();
        ~StoryTimer() = default;

         DWORD GetTime();

    private:
        void Update();
        DWORD m_currentTime;
    };


}