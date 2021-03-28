#pragma once

#ifndef WINDOW
#define WINDOW
#include <Windows.h>
#include <stdio.h>

#include "Resources/resource.h"
#define WIN32_LEAN_AND_MEAN



template <class T>
class BaseWindow
{

public:
    enum StatusCode {
        Null,
        Initializing,
        InitiailizationFailed,
        Running,
        Closing
    };

	static LRESULT CALLBACK sWindowProc(HWND, UINT, WPARAM, LPARAM);
	static const bool FULL_SCREEN;
	static const bool VSYNC_ENABLED;
	static const float SCREEN_DEPTH;
	static const float SCREEN_NEAR;

    BaseWindow(HINSTANCE hInstance)
        : mWnd(NULL)
        , mhInstance(hInstance)
        , mStatusCode(StatusCode::Null)
    {}
    virtual ~BaseWindow() {}
	bool Create(
        LPCWSTR lpWindowName,
		DWORD dwStyle,
		int& width,
		int& height,
		DWORD dwExStyle = 0,
		int X = CW_USEDEFAULT,
		int Y = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	);
	HWND GetHwnd()const { return mWnd; }

protected:
	void ShutdownWindows();
	virtual LRESULT HandMessage(UINT uMSG, WPARAM wParam, LPARAM lParam) = 0;
	virtual LPCWSTR ClassName() const = 0;

	HWND mWnd;
    HINSTANCE mhInstance;
    StatusCode mStatusCode;

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
	LPCWSTR lpWindowName,
	DWORD dwStyle,
	int& width,
	int& height,
	DWORD dwExStyle /*= 0*/,
	int X /*= CW_USEDEFAULT*/,
	int Y /*= CW_USEDEFAULT*/,
	HWND hWndParent /*= 0*/,
	HMENU hMenu /*= 0*/)
{
    mStatusCode = Initializing;
	DEVMODE dmScreenSetting;

	// This will hold the class we created
    WNDCLASSEX wc = { 0 };

    // First fill in the window class structure
    wc.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    // The size, in bytes, of this structure. Set this member to sizeof(WNDCLASSEX). Be sure to set this member before calling the GetClassInfoEx function.
    // The parameter MUST be correct, if not will due window class class cannot be created because the parameter is invalid.
    wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = T::sWindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hIcon = LoadIcon(mhInstance, MAKEINTRESOURCE(ID_SCAFFOLD_LOGO));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    //wc.hIconSm = LoadIcon(mhInstance, L"ScaffoldLogo");
    //wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    //If the function succeeds, the return value is a class atom that uniquely identifies the class being registered.
    //This atom can only be used by the CreateWindow, CreateWindowEx, GetClassInfo, GetClassInfoEx, FindWindow, FindWindowEx, 
    //and UnregisterClass functions and the IActiveIMMap::FilterClientWindows method.
    //If the function fails, the return value is zero. 
	ATOM classAtom = RegisterClassEx(&wc);
    if (classAtom == 0)
    {
        mStatusCode = StatusCode::InitiailizationFailed;
        return false;
    }

	//width = GetSystemMetrics(SM_CXSCREEN);
	//height = GetSystemMetrics(SM_CYSCREEN);
	if (BaseWindow::FULL_SCREEN)
	{
		//width = GetSystemMetrics(SM_CXSCREEN);
		//height = GetSystemMetrics(SM_CYSCREEN);

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
        if (width <= 0)
        {
            width = 800;
        }
        if (height <= 0)
        {
            height = 600;
        }


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
        mStatusCode = StatusCode::InitiailizationFailed;
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

