#include "MainWindow.h"
#include "DebugTool/DebugConsole.h"

#include "CrossPlatform/FrameBuffer.h"

#include "Windows/WindowDrawer/WindowDGIDrawer.h"
#include "Windows/WindowDrawer/DirectXDrawDrawer.h"

#include "Timer/StoryTimer.h"

#include <cstdio>
#include <TCHAR.H>
#include <cmath> // sin, cos

#include "MathLab/Vector3.h"
#include "MathLab/MathLib.h"
#include "MathLab/MathLabDefinition.h"

#include "Gameplay/GameplayMain.h"

using MathLab::Vector3;

using WindowDrawer::WindowDGIDrawer;
using WindowDrawer::WindowDGIDrawerConfig;

using WindowDrawer::DirectXDrawDrawer;
using WindowDrawer::DirectXDrawDrawerConfig;

MainWindow::MainWindow(HINSTANCE hInstance) : BaseWindow(hInstance)
, m_debugConsole(new DebugTool::DebugConsole())
, m_windowDrawer(new WindowDrawer::DirectXDrawDrawer())
, m_timer(new Timer::StoryTimer())
, m_lastUpdateTiemstamp(0)
, m_screenWidth(1920)
, m_screenHight(1080)
, m_perspectiveProjection(true)
{
}


MainWindow::~MainWindow()
{
}

void MainWindow::Initialize()
{
    m_debugConsole->Initialize();

    //bool result;
    //int screenWidth = 800;
    //int screenHight = 600;
    if (!this->Create(L"Main Windows",
        /*WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/
        WS_OVERLAPPEDWINDOW | WS_POPUP,
        m_screenWidth,
        m_screenHight,
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        LoadMenu(mhInstance, MAKEINTRESOURCE(IDR_MAIN_MENU))
    ))
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
    }
    break;

    case CrossPlatform::DrawerType::DirectX_Draw_Drawer:
    {
        DirectXDrawDrawerConfig* const config = new DirectXDrawDrawerConfig;
        config->resolutionX = m_screenWidth;
        config->resolutionY = m_screenHight;
        config->m_hwnd = GetHwnd();

        m_windowDrawer->SetDrawerConfig(config);
        bool result = m_windowDrawer->Initialize();
        if (!result)
        {
            return;
        }
    }
    break;

    default:
        break;
    }


}

void MainWindow::Shutdown()
{
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

        Gameplay::GameplayMain();

        frameAmount++;

        _stprintf_s(debugTextBuffer, 80, _T("Frame Amout: %d"), frameAmount);
        TextOut(hdc, 0, 0, debugTextBuffer, _tcslen(debugTextBuffer));

        bool isUpdateBuffer = CanUpdateBuffer();


        Vector3 cameraPosition();
        static int tmpdegree = 90;

        Vector3 lightDirection(0, 1, 0);

        if (isUpdateBuffer)
        {
            m_windowDrawer->GetFrameBuffer()->ClearBuffer();

            int donutRadius = 120;
            int circleRadius = 40;

            float degree = 0;    //(0 ~ 360)

            float X = 0;
            float Y = 0;
            float Z = 0;
            for (int thetaDegree = 0; thetaDegree < 360; thetaDegree += 5)
            {
                Vector3 circle;
                Vector3 circleNormal;
                float posX = 0;
                float posY = 0;
                float posZ = 0;

                float theta = thetaDegree * TO_RADIAN;
                circle.x = donutRadius + circleRadius * sin(theta);
                circle.y = circleRadius * cos(theta);
                circleNormal.x = sin(theta);
                circleNormal.y = cos(theta);

                //To donut, rotate with y-axis
                //(x,y,z) * |cos  , 0 ,sin|
                //          |0    ,1  ,0  |       
                //          |-sin ,0  ,cos|

                for (int phiDegree = 0; phiDegree < 360; phiDegree += 5)
                {
                    Vector3 donutVertex = MathLab::RotateYAxis(circle, (float)phiDegree);
                    Vector3 donutVertexNormal = MathLab::RotateYAxis(circleNormal, (float)phiDegree);
                    //float phi = phiDegree * TO_RADIAN;
                    //X = posX * cos(phi) - posZ * sin(phi);
                    //Y = posY;
                    //Z = posY * sin(phi) + posZ * cos(phi);

                    //local
                    //Vector3 donutVertex(X, Y, Z);

                    donutVertex = MathLab::RotateXAxis(donutVertex, (float)tmpdegree);
                    donutVertexNormal = MathLab::RotateXAxis(donutVertexNormal, (float)tmpdegree);
                    //local to world
                    donutVertex = MathLab::RotateZAxis(donutVertex, tmpdegree);
                    donutVertexNormal = MathLab::RotateZAxis(donutVertexNormal, (float)tmpdegree);
                    donutVertex += Vector3(0, 0, -170);

                    //local to view 


                    //view to projection
                    int screenX = 0;
                    int screenY = 0;

                    if (donutVertex.z >= -1)
                    {
                        continue;
                    }
                    float reciprocalOfZ = -1 / donutVertex.z;

                    // Rasterized
                    if (m_perspectiveProjection)
                    {
                        screenX = (int)m_screenWidth / 2 + donutVertex.x * reciprocalOfZ * 100;
                        screenY = (int)m_screenHight / 2 - donutVertex.y * reciprocalOfZ * 100;
                    }
                    else {
                        screenX = (int)donutVertex.x + m_screenWidth / 2;
                        screenY = (int)-(donutVertex.y) + m_screenHight / 2;
                    }

                    // Z buffer
                    float curretZBuffer = m_windowDrawer->GetFrameBuffer()->GetZBuffer(screenX, screenY);
                    if (curretZBuffer < reciprocalOfZ)
                    {
                        float L = donutVertexNormal.dot(lightDirection);
                        L += 0.5f;
                        if (L > 0)
                        {
                            L *= 255; //scale up to 255
                            L += 20;
                            L = MathLab::clamp(L, 0.0f, 255.0f);
                            m_windowDrawer->GetFrameBuffer()->SetZBuffer(screenX, screenY, reciprocalOfZ);
                            m_windowDrawer->GetFrameBuffer()->SetColor(screenX, screenY, CrossPlatform::SColorRGB(L, L, L));

                        }
                    }

                }
            }



            //Center Position
            int centerX = 0;
            int centerY = 0;
            int centerZ = 0;


            //for (int i = 0 ; i< 800; i++)
            //{
            //    for (int j = 0; j < 600; j++)
            //    {
            //        m_windowDrawer->GetFrameBuffer()->SetColor(i, j, CrossPlatform::SColorRGB(100,100,100));
            //    }
            //}
            //m_windowDrawer->GetFrameBuffer()->SetColor(10, 10, CrossPlatform::SColorRGB(20, 20, 20));
            //m_windowDrawer->GetFrameBuffer()->SetColor(11, 11, CrossPlatform::SColorRGB(20, 20, 20));
            //m_windowDrawer->GetFrameBuffer()->SetColor(12, 12, CrossPlatform::SColorRGB(20, 20, 20));
            //m_windowDrawer->GetFrameBuffer()->SetColor(13, 13, CrossPlatform::SColorRGB(20, 20, 20));
            //m_windowDrawer->GetFrameBuffer()->SetColor(14, 14, CrossPlatform::SColorRGB(20, 20, 20));
            //m_windowDrawer->GetFrameBuffer()->SetColor(15, 15, CrossPlatform::SColorRGB(20, 20, 20));
            m_windowDrawer->Draw();
        }

        tmpdegree += 5;
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

        if (virtualCode == 0x50) //P
        {
            m_perspectiveProjection = !m_perspectiveProjection;
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
    time_t currentTime = m_timer->GetTime();
    double timeInterval = difftime(currentTime, m_lastUpdateTiemstamp);
    if (timeInterval > m_timeInterval)
    {
        m_lastUpdateTiemstamp = currentTime;
        return true;
    }
    return false;
}
