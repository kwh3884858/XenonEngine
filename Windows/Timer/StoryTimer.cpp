#include "StoryTimer.h"



namespace Timer {

    void StoryTimer::Update()
    {
        m_currentTime = GetTime();
    }

    //void HeavenGateEditor::StoryTimer<T>::AddCallback(int interval,)
    //{
    //    m_interval = interval;
    //    m_lastTime = GetTime();
    //}

    DWORD StoryTimer::GetTime()
    {
        return GetTickCount();
    }

    StoryTimer::StoryTimer()
    {
        Update();
    }

}
