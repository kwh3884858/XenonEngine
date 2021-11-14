//
//Copyright (c) 2019 Star Platinum
//
//Created by Kong Wei Hang, 2019-12-10
//example_win32_directx11
//
//Add Description
//
#pragma once

namespace XenonEngine {

#define COMBINE(left, right) left##right

#define STR(str) #str

#define WINDOW_DECLARE_NAME(windowName) \
public: \
    const char*         GetWindiwName() const override          { return windowName; } 

#define WINDOW_DECLARE_TYPE(windowType) \
public: \
    Window_Type         GetWindowType() const override          { return windowType; } 

    class XenonBaseWindow
    {
    public:
        enum class Window_Type
        {
            MultiWindow,
            SingleWindow,
            SubWindow,
            Popup
        };
        XenonBaseWindow();
        XenonBaseWindow(const XenonBaseWindow&) = delete;
        XenonBaseWindow(XenonBaseWindow* parent);
        virtual ~XenonBaseWindow();

        void Update(const void* data = nullptr);

        bool*const GetHandle();
        void OpenWindow();
        void CloseWindow();
        bool IsWindowOpen() const;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        int GetWindowIndex() const { return m_index; }

    protected:
        virtual void UpdateMainWindow(const void* data = nullptr);
        //virtual void UpdateMenu();
        virtual const char* GetWindiwName() const = 0;
        virtual Window_Type GetWindowType() const = 0;
        void SetParentWindow(XenonBaseWindow * parent);

    protected:
        static int indexCount;
        bool m_open;
        XenonBaseWindow *  m_parent;
        int m_index;
    };
}
