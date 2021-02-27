#pragma once

#include <time.h>

namespace Timer {

    //class HeavenGateWindowStoryEditor;

    class StoryTimer final
    {
    public:
        //typedef  void(T::*StoryEditorCallback)();
        StoryTimer() = default;
        ~StoryTimer() = default;

         void Update();
         time_t GetTime();

        //static void AddCallback(int interval, T* storyEditor, StoryEditorCallback callback);

    private:
        time_t m_currentTime;
        //static time_t m_interval;
        //static time_t m_lastTime;

        //static StoryEditorCallback m_timeCallback;
        //static T* m_this;
    };

//    template<typename T>
//#ifdef _WIN32
//    __declspec(selectany)
//#endif
//        T* StoryTimer<T>::m_this = nullptr;
//
//
//    template<typename T>
//#ifdef _WIN32
//    __declspec(selectany)
//#endif
//        typename StoryTimer<T>::StoryEditorCallback StoryTimer<T>::m_timeCallback = nullptr;
//
    //__declspec(selectany) void (T::*StoryTimer<T>::m_timeCallback) () = nullptr;



//#ifdef _WIN32
//    __declspec(selectany)
//#endif
//        time_t StoryTimer::m_lastTime = 0;

//
//#ifdef _WIN32
//    __declspec(selectany)
//#endif
//        time_t StoryTimer::m_currentTime = 0;
//
//
    //template<typename T>
    //HeavenGateEditor::StoryTimer<T>::StoryTimer()
    //{
    //    m_lastTime = 0;
    //    m_interval = 0;
    //    m_timeCallback = nullptr;
    //    
    //    return true;
    //}

    //template<typename T>
    //HeavenGateEditor::StoryTimer<T>::~StoryTimer()
    //{
    //    m_lastTime = 0;
    //    m_interval = 0;
    //    m_timeCallback = nullptr;

    //    return true;
    //}


}