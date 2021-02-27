#include "StoryTimer.h"



namespace HeavenGateEditor {


    template<typename T>
    void HeavenGateEditor::StoryTimer::Update()
    {
        if (m_this == nullptr)
        {
            return;
        }

        time_t currentTime = GetTime();

    }

    //void HeavenGateEditor::StoryTimer<T>::AddCallback(int interval,)
    //{
    //    m_interval = interval;
    //    m_lastTime = GetTime();
    //}

    time_t HeavenGateEditor::StoryTimer<T>::GetTime()
    {
        return time(nullptr);
    }

}
