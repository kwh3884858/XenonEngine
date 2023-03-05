#pragma once

#include "BaseWindow.h"

namespace CrossPlatform {
    class IDrawer;
    class IDrawerSurface;
    class IFileReader;
}

namespace DebugTool {
    class DebugConsole;
}

namespace Timer {
    class StoryTimer;
}

namespace WindowInput {
    class DirectXInput;
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
    CrossPlatform::IDrawer* m_windowDrawer;
    CrossPlatform::IDrawerSurface* m_directXDrawSurface;
    CrossPlatform::IDrawerSurface* m_zBuffer;
    CrossPlatform::IFileReader* m_fileReader;
    WindowInput::DirectXInput* m_directInput;

    double m_timeInterval = 16.66;  //60fps
    Timer::StoryTimer* m_timer;

    int m_screenWidth;
    int m_screenHight;
    bool m_active;

};