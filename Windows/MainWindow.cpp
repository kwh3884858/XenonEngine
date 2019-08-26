#include "MainWindow.h"



MainWindow::MainWindow()
{
}


MainWindow::~MainWindow()
{
}

void MainWindow::Initialize()
{
	bool result;
	int screenWidth;
	int screenHight;
	if (!this->Create(L"Main Windows",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		screenWidth,
		screenHight
	)) {
		return;
	}

}

void MainWindow::Shutdown()
{
	ShutdownWindows();
	return;
}

void MainWindow::Run()
{
	MSG msg = {};
	bool done = false, result = false, isKey = false;

	ZeroMemory(&msg, sizeof(MSG));
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
		
			if (result)
			{
				done = true;
				break;
			}
		}

	}

	return;
}
LRESULT MainWindow::HandMessage(UINT uMSG, WPARAM wParam, LPARAM lParam)
{
	switch (uMSG)
	{
		//case WM_KEYDOWN:
		//{
		//	mInputDx->setKeyDown((unsigned int)wParam);
		//	return 0;
		//}
		//case WM_KEYUP:
		//{
		//	mInputDx->setKeyUp((unsigned int)wParam);
		//	return 0;
		//}
		//case WM_MOUSEMOVE:
		//{
		//	mInputDx->setMousePos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		//	return 0;
		//}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(mWnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(mWnd, &ps);
		//std::cout << "Redraw!" << std::endl;
	}
	return 0;

	case WM_CLOSE:
	{
		if (MessageBox(mWnd, L"Ready Quit?", L"Application", MB_OKCANCEL) == IDOK) {
			DestroyWindow(mWnd);
		}
	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(mWnd, uMSG, wParam, lParam);
	}

	return TRUE;
}