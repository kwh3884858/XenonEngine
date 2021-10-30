#include "StoryTimer.h"
#include <chrono>

namespace Timer {

    void StoryTimer::Update()
    {
        m_currentTime = std::chrono::system_clock::now();
    }

    StoryTimer::StoryTimer()
    {
        Update();
    }

    //float StoryTimer::GetTime() const
    //{
        //return GetTickCount();
    //}

    double StoryTimer::GetTimeMilliSecond() const
    {

        milliseconds runtime = std::chrono::duration_cast<milliseconds> ( std::chrono::system_clock::now() - m_currentTime );
        return runtime.count();
    }

}
