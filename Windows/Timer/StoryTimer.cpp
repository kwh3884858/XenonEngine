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

    long StoryTimer::GetTimeMilliSecond() const
    {

		std::chrono::milliseconds runtime = std::chrono::duration_cast<std::chrono::milliseconds> ( std::chrono::system_clock::now() - m_currentTime );
        return (long) runtime.count();
    }

}
