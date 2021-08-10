#include "MainWindow.h"
#include "DebugTool/DebugConsole.h"

//#include "CrossPlatform/SColorRGBA.h"

#include "CrossPlatform/Database.h"

#include "Windows/WindowDrawer/WindowDGIDrawer.h"
#include "Windows/WindowDrawer/DirectXDrawDrawer.h"
#include "Windows/Surface/DirectXDrawSurface.h"
#include "Windows/Surface/DrawerSurface.h"

#include "Timer/StoryTimer.h"

#include <cstdio>
#include <TCHAR.H>
#include <assert.h>

#include "Engine/Primitive/Graphic2D.h"
#include "Engine/GameplayMain.h"

#include "Engine/FileManager/FileManager.h"
#include "Windows/File/FileReader.h"

#include "Engine/IO/InputSystem.h"
#include "Windows/Input/DirectXInput.h"

#include "Engine/Timer/XenonTimer.h"

//
//using MathLab::Vector3f;

using WindowDrawer::WindowDGIDrawer;
using WindowDrawer::WindowDGIDrawerConfig;

using WindowDrawer::DirectXDrawDrawer;
using WindowDrawer::DirectXDrawDrawerConfig;

using CrossPlatform::IDrawerSurface;
using WindowSurface::DrawerSurface;
using WindowSurface::DirectXDrawSurface;

using CrossPlatform::Database;
using CrossPlatform::ITimer;

MainWindow::MainWindow(HINSTANCE hInstance) : BaseWindow(hInstance)
, m_debugConsole(nullptr)
, m_windowDrawer(nullptr)
, m_timer(nullptr)
, m_directXDrawSurface(nullptr)
, m_lastUpdateTiemstamp(0)
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
    m_lastUpdateTiemstamp = m_timer->GetTime();

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
        bool result = m_windowDrawer->Initialize();
        assert(result == true);

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
    primitive2DConfig.m_MinDrawPosition = Vector2f(Database::Get().engineConfig.m_minX, Database::Get().engineConfig.m_minY);
    primitive2DConfig.m_MaxDrawPosition = Vector2f(Database::Get().engineConfig.m_maxX, Database::Get().engineConfig.m_maxY);
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
    bool done = false, result = false, isKey = false;
    unsigned long int frameAmount = 0;

    //Color
    COLORREF colorRed = RGB(255, 0, 0);
    COLORREF colorGreen = RGB(0, 255, 0);
    COLORREF colorBlue = RGB(0, 0, 255);

    //Text
    SetTextColor(hdc, colorRed);
    SetBkColor(hdc, colorGreen);
    SetBkMode(hdc, OPAQUE);

    //Debug text
    TCHAR debugTextBuffer[80];

    //Game Init
    Gameplay::GameplayInitialize();

    ZeroMemory(&msg, sizeof(MSG));
    if (mStatusCode == BaseWindow::StatusCode::InitiailizationFailed)
    {
        m_debugConsole->RetrieveError(_T("BaseWindow::Create"));
    }
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
        frameAmount++;
        _stprintf_s(debugTextBuffer, 80, _T("Frame Amout: %d"), frameAmount);
        TextOut(hdc, 0, 0, debugTextBuffer, _tcslen(debugTextBuffer));

        bool isUpdateBuffer = CanUpdateBuffer();

        if (isUpdateBuffer)
        {
            m_directXDrawSurface->lock();
            m_zBuffer->lock();
            m_directInput->Update();
            Gameplay::GameplayMain();
            m_directXDrawSurface->Unlock();
            m_zBuffer->Unlock();
            bool result = m_windowDrawer->Draw(m_directXDrawSurface);
        }
    }
    Gameplay::GameplayShutdown();
    return;
}



LRESULT MainWindow::HandMessage(UINT uMSG, WPARAM wParam, LPARAM lParam)
{
    static unsigned long int paintAmount = 0;
    static TCHAR debugTextBuffer2[80];

    COLORREF colorRed = RGB(255, 0, 0);
    COLORREF colorGreen = RGB(0, 255, 0);
    COLORREF colorBlue = RGB(0, 0, 255);

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

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        // Make the full window invalid
        InvalidateRect(GetHwnd(), nullptr, false);
        HDC hdc = BeginPaint(mWnd, &ps);

        paintAmount++;

        _stprintf_s(debugTextBuffer2, 80, _T("WM Paint Amout: %d"), paintAmount);
        TextOut(hdc, 0, 10, debugTextBuffer2, _tcslen(debugTextBuffer2));


        EndPaint(mWnd, &ps);
        //std::cout << "Redraw!" << std::endl;
    }
    break;

    case WM_KEYDOWN:
    {
        int virtualCode = (int)wParam;
        int keyState = (int)lParam;

        if (virtualCode == 0x50) //P
        {
            Database::Get().engineConfig.m_isPerspectiveProjection = !Database::Get().engineConfig.m_isPerspectiveProjection;
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

    // WM_QUIT

    default:
        return DefWindowProc(mWnd, uMSG, wParam, lParam);
    }

    return TRUE;
}

bool MainWindow::CanUpdateBuffer()
{
    DWORD currentTime = m_timer->GetTime();
    DWORD timeInterval = currentTime-m_lastUpdateTiemstamp;

    if (timeInterval > m_timeInterval)
    {
        float fps = 1 / timeInterval;
        TCHAR debugTextBuffer[80];
        _stprintf_s(debugTextBuffer, 80, _T("FPS: %d"), (int)fps);
        TextOut(hdc, 0, 40, debugTextBuffer, _tcslen(debugTextBuffer));
        printf("%lc", debugTextBuffer);

        m_lastUpdateTiemstamp = currentTime;
        return true;
    }
    return false;
}
