#include "MainWindow.h"
#include "DebugTool/DebugConsole.h"

#include "CrossPlatform/Database.h"

#include "Windows/WindowDrawer/WindowDGIDrawer.h"
#include "Windows/WindowDrawer/DirectXDrawDrawer.h"
#include "Windows/Surface/DirectXDrawSurface.h"
#include "Windows/Surface/DrawerSurface.h"
#include "Windows/File/FileReader.h"
#include "Windows/Input/DirectXInput.h"

#include "Timer/StoryTimer.h"

#include <cstdio>
#include <TCHAR.H>
#include <assert.h>

#include "Engine/Graphic/Graphic2D.h"
#include "Engine/FileManager/FileManager.h"
#include "Engine/Timer/XenonTimer.h"
#include "Engine/EngineManager.h"
#include "Engine/IO/InputSystem.h"

using WindowDrawer::WindowDGIDrawer;
using WindowDrawer::WindowDGIDrawerConfig;

using WindowDrawer::DirectXDrawDrawer;
using WindowDrawer::DirectXDrawDrawerConfig;

using CrossPlatform::IDrawerSurface;
using WindowSurface::DrawerSurface;
using WindowSurface::DirectXDrawSurface;

using CrossPlatform::Database;
using CrossPlatform::ITimer;

using XenonEngine::EngineManager;

MainWindow::MainWindow(HINSTANCE hInstance) : BaseWindow(hInstance)
, m_debugConsole(nullptr)
, m_windowDrawer(nullptr)
, m_timer(nullptr)
, m_directXDrawSurface(nullptr)
, m_screenWidth(Database::Get().engineConfig.m_width)
, m_screenHight(Database::Get().engineConfig.m_height)
, m_directInput(nullptr)
//, m_isFullScreen(false)
//, m_perspectiveProjection(true)
{
}


MainWindow::~MainWindow()
{

}

void MainWindow::Initialize()
{
    
    m_windowDrawer = new WindowDrawer::DirectXDrawDrawer();

    m_timer = new Timer::StoryTimer();

    m_debugConsole = new DebugTool::DebugConsole();
    m_debugConsole->Initialize();

    if (Database::Get().engineConfig.m_isFullScreen)
    {
        m_screenWidth = GetSystemMetrics(SM_CXSCREEN);
        m_screenHight = GetSystemMetrics(SM_CYSCREEN);
    }
    //bool result;
    //int screenWidth = 800;
    //int screenHight = 600;
    if (!this->Create(L"Xenon Engine",
        /*WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/
        Database::Get().engineConfig.m_isFullScreen ? (WS_POPUP) : (/*WS_OVERLAPPED | WS_POPUP*/ WS_OVERLAPPEDWINDOW),
        m_screenWidth,
        m_screenHight,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        LoadMenu(mhInstance, MAKEINTRESOURCE(IDR_MAIN_MENU)),
        Database::Get().engineConfig.m_isFullScreen)
    )
    {
        m_debugConsole->RetrieveError(_T("MainWindow::Initialize"));
        //printf("Error Code: %d", GetLastError());
        return;
    }

    switch (m_windowDrawer->GetType())
    {
    case CrossPlatform::DrawerType::GDI_Drawer:
    {
        WindowDGIDrawer* drawer = static_cast<WindowDGIDrawer*>(m_windowDrawer);

        WindowDGIDrawerConfig* const config = new WindowDGIDrawerConfig;
        config->resolutionX = m_screenWidth;
        config->resolutionY = m_screenHight;

        //FramerBufferHandler framerBufferHandler = new CrossPlatform::FramerBuffer();
        drawer->SetDrawerConfig(config);
        drawer->Initialize();
        //m_windowDrawer->SetFrameBufeer(framerBufferHandler);

        hdc = GetDC(GetHwnd());
        drawer->SetHDC(hdc);

        m_directXDrawSurface = new WindowSurface::DrawerSurface(m_screenWidth, m_screenHight);
        m_zBuffer = new WindowSurface::DrawerSurface(m_screenWidth, m_screenHight);
    }
    break;

    case CrossPlatform::DrawerType::DirectX_Draw_Drawer:
    {
        DirectXDrawDrawerConfig* const config = new DirectXDrawDrawerConfig;
        config->resolutionX = m_screenWidth;
        config->resolutionY = m_screenHight;
        config->m_hwnd = GetHwnd();
        config->m_isFullScreen = Database::Get().engineConfig.m_isFullScreen;

        m_windowDrawer->SetDrawerConfig(config);
        assert(m_windowDrawer->Initialize() == true);

        DirectXDrawDrawer* const directDrawer = static_cast<DirectXDrawDrawer*> (m_windowDrawer);
        LPDIRECTDRAW7 lpdd = directDrawer->GetDirectRaw();
        m_directXDrawSurface = new WindowSurface::DirectXDrawSurface(lpdd, m_screenWidth, m_screenHight, DDLOCK_SURFACEMEMORYPTR);
        m_zBuffer = new WindowSurface::DirectXDrawSurface(lpdd, m_screenWidth, m_screenHight, DDLOCK_SURFACEMEMORYPTR);
    }
    break;

    default:
        break;
    }
	XenonEngine::Primitive2DConfig primitive2DConfig;
    primitive2DConfig.m_drawerSurface = m_directXDrawSurface;
    primitive2DConfig.m_zBuffer = m_zBuffer;
    primitive2DConfig.m_MinDrawPosition = MathLab::Vector2i(Database::Get().engineConfig.m_minX, Database::Get().engineConfig.m_minY);
    primitive2DConfig.m_MaxDrawPosition = MathLab::Vector2i(Database::Get().engineConfig.m_maxX, Database::Get().engineConfig.m_maxY);
	XenonEngine::Graphic2D::Get().SetConfig(&primitive2DConfig);

    m_fileReader = new File::FileReader;
    XenonEngine::FileManager::Get().SetFileReader(m_fileReader);

    WindowInput::DirectXInputConfig inputConfig;
    inputConfig.m_hwnd = GetHwnd();
    inputConfig.m_mainInstance = mhInstance;
    m_directInput = new WindowInput::DirectXInput;
    m_directInput->SetConfig(&inputConfig);
    m_directInput->Initialize();
    XenonEngine::InputSystem::Get().SetFileReader(m_directInput);

	Timer::StoryTimer* timer = new Timer::StoryTimer;
	XenonEngine::XenonTimer::Get().SetFileReader(timer);
}

void MainWindow::Shutdown()
{
    m_directInput->ShutDown();
	XenonEngine::XenonTimer::Get().Shutdown();
    XenonEngine::FileManager::Get().Shutdown();
	XenonEngine::Graphic2D::Get().Shutdown();

    delete m_directInput;
    m_directInput = nullptr;

    delete m_fileReader;
    m_fileReader = nullptr;

    delete m_directXDrawSurface;
    m_directXDrawSurface = nullptr;
    
    delete m_zBuffer;
    m_zBuffer = nullptr;

    ReleaseDC(GetHwnd(), hdc);
    ShutdownWindows();
    m_debugConsole->Shutdown();
    m_windowDrawer->Shutdown();
    return;
}

void MainWindow::Run()
{
    MSG msg = {};
    bool done = false;
    //unsigned long int frameAmount = 0;

    //Color
    COLORREF colorRed = RGB(255, 0, 0);
    COLORREF colorGreen = RGB(0, 255, 0);
    //COLORREF colorBlue = RGB(0, 0, 255);

    //Text
    SetTextColor(hdc, colorRed);
    SetBkColor(hdc, colorGreen);
    SetBkMode(hdc, OPAQUE);

    //Game Init
    EngineManager::Get().Initialize();

    bool isGotMessage = false;

    ZeroMemory(&msg, sizeof(MSG));
    if (mStatusCode == BaseWindow::StatusCode::InitiailizationFailed)
    {
        m_debugConsole->RetrieveError(_T("BaseWindow::Create"));
    }
    while (!done)
    {
		if (m_active)
			isGotMessage = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		else
            isGotMessage = GetMessage(&msg, NULL, 0U, 0U);
        if (isGotMessage)
        {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
        }
        // Handle the windows messages.
        //if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        //{

        //}

        // If windows signals to end the application then exit out.
        if (msg.message == WM_QUIT)
        {
            done = true;
        }

        long timeInterval = m_timer->GetTimeMilliSecond();
        // Input
		m_directInput->Update();

        // Editor
        EngineManager::Get().Update(timeInterval);

        // Graphic, 60fps
        if (timeInterval > m_timeInterval)
        {
            m_timer->Update();

			/*bool result = */
			if (m_active)
			{
				m_directXDrawSurface->lock();
				m_zBuffer->lock();


				if (m_windowDrawer->GetType() == CrossPlatform::DrawerType::DirectX_Draw_Drawer)
				{

					HDC workingDC = nullptr;
					DirectXDrawSurface* directXDrawSurface = static_cast<DirectXDrawSurface*>(m_directXDrawSurface);
					assert(directXDrawSurface->GetDirectRawSurface()->GetDC(&workingDC) == DD_OK);

					static TCHAR debugTextBuffer2[80];
					static unsigned long int paintAmount = 0;
					paintAmount++;
					_stprintf_s(debugTextBuffer2, 80, _T("WM Paint Amout: %d"), paintAmount);
					TextOut(workingDC, 0, 10, debugTextBuffer2, (int)_tcslen(debugTextBuffer2));
					SetTextColor(workingDC, colorRed);

					long secondPerFrame = timeInterval / 1000;
					TCHAR debugTimeInterval[80];
					_stprintf_s(debugTimeInterval, 80, _T("Time per frame: %ld s"), secondPerFrame);
					TextOut(workingDC, 0, 30, debugTimeInterval, (int)_tcslen(debugTimeInterval));

					double fps = 1000.0f / timeInterval;
					TCHAR debugTextBuffer[80];
					_stprintf_s(debugTextBuffer, 80, _T("FPS: %f"), fps);
					TextOut(workingDC, 0, 50, debugTextBuffer, (int)_tcslen(debugTextBuffer));

					directXDrawSurface->GetDirectRawSurface()->ReleaseDC(workingDC);
				}
				m_zBuffer->Unlock();
				m_directXDrawSurface->Unlock();

				m_windowDrawer->Draw(m_directXDrawSurface);
			}

        }
    }
    EngineManager::Get().Shutdown();
    return;
}



LRESULT MainWindow::HandMessage(UINT uMSG, WPARAM wParam, LPARAM lParam)
{
    //COLORREF colorRed = RGB(255, 0, 0);
    //COLORREF colorGreen = RGB(0, 255, 0);
    //COLORREF colorBlue = RGB(0, 0, 255);

	//TODO: only work after imgui is enable.
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	//{
	//	return true;
	//}

    switch (uMSG)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_CLOSE:
        {
            PostMessage(GetHwnd(), WM_CLOSE, 0, 0);
        }
        break;
        default:
            break;
        }

	case WM_SIZE:
		// Check to see if we are losing our window...
		if (SIZE_MAXHIDE == wParam || SIZE_MINIMIZED == wParam)
			m_active = false;
		else
            m_active = true;
		break;

    //case WM_PAINT:
    //{
        //PAINTSTRUCT ps;
        //InvalidateRect(GetHwnd(), nullptr, false);
        //HDC hdc = BeginPaint(mWnd, &ps);

        //paintAmount++;

        //_stprintf_s(debugTextBuffer2, 80, _T("WM Paint Amout: %d"), paintAmount);
        //TextOut(hdc, 0, 10, debugTextBuffer2, _tcslen(debugTextBuffer2));

        //EndPaint(mWnd, &ps);
    //}
    //break;

    case WM_KEYDOWN:
    {
        int virtualCode = (int)wParam;
        //int keyState = (int)lParam;

        if (virtualCode == 0x50) //P
        {
            //Database::Get().engineConfig.m_isPerspectiveProjection = !Database::Get().engineConfig.m_isPerspectiveProjection;
        }
    }
    break;

    // It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
    {
        if (MessageBox(mWnd, _T("Ready Quit?"), _T("Application"), MB_OKCANCEL) == IDOK) {
            DestroyWindow(mWnd);
        }
    }
    break;

	//TODO: only work after imgui is enable.
	//case WM_SIZE:
	//	if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
	//	{
	//		g_d3dpp.BackBufferWidth = LOWORD(lParam);
	//		g_d3dpp.BackBufferHeight = HIWORD(lParam);
	//		ResetDevice();
	//	}
	//	return 0;

	//case WM_SYSCOMMAND:
	//	if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
	//		return 0;
	//	break;


    // WM_QUIT
    default:
        return DefWindowProc(mWnd, uMSG, wParam, lParam);
    }

    return TRUE;
}

bool MainWindow::CanUpdateBuffer()
{
    return false;
}
