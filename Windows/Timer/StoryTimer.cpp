#include "StoryTimer.h"

namespace Timer {

    void StoryTimer::Update()
    {
        m_currentTime = GetTime();
    }

    StoryTimer::StoryTimer()
    {
        Update();
    }

    float StoryTimer::GetTime() const
    {
        return GetTickCount();
    }
}
