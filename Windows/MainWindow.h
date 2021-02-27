#pragma once

#include "BaseWindow.h"
#include <time.h>

namespace DebugTool {
    class DebugConsole;
}

namespace WindowDrawer {
    class WindowDrawer;
}

namespace Timer {
    class StoryTimer;
}

class MainWindow : public BaseWindow<MainWindow>
{
public:
	MainWindow(HINSTANCE hInstance);
	virtual ~MainWindow() override;

	void Initialize();
	void Shutdown();
	void Run();

	virtual LRESULT HandMessage(UINT uMSG, WPARAM wParam, LPARAM lParam) override;

protected:

	virtual LPCWSTR ClassName() const override { return L"Main Window Class"; }

private:
    bool CanUpdateBuffer();

    DebugTool::DebugConsole* m_debugConsole;
    HDC hdc;
    WindowDrawer::WindowDrawer* m_windowDrawer;
    
    double m_timeInterval = 0.0333333;  //30fps
    Timer::StoryTimer m_timer;
    time_t m_lastUpdateTiemstamp;
};