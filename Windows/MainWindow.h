#pragma once

#include "BaseWindow.h"
#include "WindowDrawer/WindowDrawer.h"

namespace DebugTool {
    class DebugConsole;
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
    DebugTool::DebugConsole& m_debugConsole;
    HDC hdc;
    WindowDrawer& m_windowDrawer;
};