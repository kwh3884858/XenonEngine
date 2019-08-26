#pragma once

#ifndef WINDOW
#define WINDOW
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN



template <class T>
class BaseWindow
{
public:
	static LRESULT CALLBACK sWindowProc(HWND, UINT, WPARAM, LPARAM);
	static const bool FULL_SCREEN;
	static const bool VSYNC_ENABLED;
	static const float SCREEN_DEPTH;
	static const float SCREEN_NEAR;

	BaseWindow() : mWnd(NULL) {}
	bool Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		int& width,
		int& height,
		DWORD dwExStyle = 0,
		int X = CW_USEDEFAULT,
		int Y = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	);
	HWND getHwnd()const { return mWnd; }




protected:
	void ShutdownWindows();
	virtual LRESULT HandMessage(UINT uMSG, WPARAM wParam, LPARAM lParam) = 0;
	virtual PCWSTR ClassName() const = 0;

	HWND mWnd;

private:

};


#endif // !WINDOW

template<class T>
const bool BaseWindow<T>::FULL_SCREEN = false;

template<class T>
const bool BaseWindow<T>::VSYNC_ENABLED = true;

template<class T>
const float BaseWindow<T>::SCREEN_DEPTH = 1000.0f;

template<class T>
const float BaseWindow<T>::SCREEN_NEAR = 0.1f;

template<class T>
bool BaseWindow<T>::Create(
	PCWSTR lpWindowName,
	DWORD dwStyle,
	int& width,
	int& height,
	DWORD dwExStyle /*= 0*/,
	int X /*= CW_USEDEFAULT*/,
	int Y /*= CW_USEDEFAULT*/,
	HWND hWndParent /*= 0*/,
	HMENU hMenu /*= 0*/)
{
	DEVMODE dmScreenSetting;

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = T::sWindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();

	RegisterClass(&wc);

	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	if (BaseWindow::FULL_SCREEN)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);

		memset(&dmScreenSetting, 0, sizeof(dmScreenSetting));
		dmScreenSetting.dmSize = sizeof(dmScreenSetting);
		dmScreenSetting.dmPelsWidth = (unsigned long)width;
		dmScreenSetting.dmPelsHeight = (unsigned long)height;
		dmScreenSetting.dmBitsPerPel = 32;
		dmScreenSetting.dmFields = DM_BITSPERPEL | DM_PAPERWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSetting, CDS_FULLSCREEN);
		X = Y = 0;
	}
	else
	{
		width = 800;
		height = 600;

		X = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		Y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	mWnd = CreateWindowEx(
		dwExStyle,
		ClassName(),
		lpWindowName,
		dwStyle,

		X, Y, width, height,

		hWndParent,
		hMenu,
		GetModuleHandle(NULL),
		this
	);
	if (!mWnd)
	{
		return false;
	}
	ShowWindow(mWnd, SW_SHOW);
	SetForegroundWindow(mWnd);
	SetFocus(mWnd);

	//ShowCursor(false);

	return true;

}

template<class T>
void BaseWindow<T>::ShutdownWindows()
{
	ShowCursor(false);
	
	//TODO: add graphic setting
	if (BaseWindow::FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(mWnd);
	mWnd = NULL;

	UnregisterClass(ClassName(), GetModuleHandle(NULL));

}


template<class T>
LRESULT BaseWindow<T>::sWindowProc(HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParam)
{
	static T* pThis = NULL;

	if (uMSG == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*> (lParam);
		pThis = reinterpret_cast<T*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

		pThis->mWnd = hwnd;
	}
	else
	{
		pThis = reinterpret_cast<T*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pThis) {

		return pThis->HandMessage(uMSG, wParam, lParam);

	}
	else
	{
		return DefWindowProc(hwnd, uMSG, wParam, lParam);
	}

}

