#include "MainWindow.h"
#include "DebugTool/DebugConsole.h"



#include <cstdio>
#include <TCHAR.H>

MainWindow::MainWindow(HINSTANCE hInstance) : BaseWindow(hInstance)
, m_debugConsole(DebugTool::DebugConsole())
{
}


MainWindow::~MainWindow()
{
}

void MainWindow::Initialize()
{
    m_debugConsole.Initialize();

    //bool result;
    int screenWidth = 800;
    int screenHight = 600;
    if (!this->Create(L"Main Windows",
        /*WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/
        WS_OVERLAPPEDWINDOW | WS_POPUP,
        screenWidth,
        screenHight,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        LoadMenu(mhInstance, MAKEINTRESOURCE(IDR_MAIN_MENU))
    ))
    {
        m_debugConsole.RetrieveError(L"MainWindow::Initialize");
        //printf("Error Code: %d", GetLastError());
        return;
    }

    FramerBufferHandler const framerBufferHandler = new FramerBuffer();
    framerBufferHandler->Initilize(screenWidth, screenHight);
    m_windowDrawer.SetFrameBufeer(framerBufferHandler);
    m_windowDrawer.SetHDC(hdc);

}

void MainWindow::Shutdown()
{
    ReleaseDC(GetHwnd(), hdc);
    ShutdownWindows();
    m_debugConsole.Shutdown();
    return;
}

void MainWindow::Run()
{
    MSG msg = {};
    bool done = false, result = false, isKey = false;
    
    unsigned long int frameAmount = 0;
    hdc = GetDC(GetHwnd());

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

    ZeroMemory(&msg, sizeof(MSG));
    if (mStatusCode == BaseWindow::StatusCode::InitiailizationFailed)
    {
        m_debugConsole.RetrieveError(L"BaseWindow::Create");
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

        
    }

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

        //SetTextColor(hdc, colorBlue);
        //SetBkColor(hdc, colorRed);
        //SetBkMode(hdc, OPAQUE);

        paintAmount++;

        _stprintf_s(debugTextBuffer2, 80, _T("WM Paint Amout: %d"), paintAmount);
        TextOut(hdc, 0, 10, debugTextBuffer2, _tcslen(debugTextBuffer2));


        //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(mWnd, &ps);
        //std::cout << "Redraw!" << std::endl;
    }
    break;

    case WM_KEYDOWN:
    {
        int virtualCode = (int)wParam;
        int keyState = (int)lParam;
    }
    break;

    // It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
    {
        if (MessageBox(mWnd, L"Ready Quit?", L"Application", MB_OKCANCEL) == IDOK) {
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