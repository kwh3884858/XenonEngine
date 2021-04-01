#include "Windows/WindowDrawer/DirectXDrawDrawer.h"
#include "Windows/Surface/DirectXDrawSurface.h"
//#include <windows.h>
#include "MathLab/MathLib.h"

#include <assert.h>

//Test
//#include "CrossPlatform/SColorRGBA.h"
//#include <cstdio>


namespace WindowDrawer {
    using WindowSurface::DirectXDrawSurface;
    
#define DDRAW_INIT_STRUCT(dxstruct)  memset(&dxstruct, 0, sizeof(dxstruct)); dxstruct.dwSize = sizeof(dxstruct);// that should all be one line
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)
    DirectXDrawDrawer::~DirectXDrawDrawer()
    {

    }

    void DirectXDrawDrawer::SetDrawerConfig(IDrawerConfig* const config)
    {
        if (!config)
        {
            return;
        }

        DirectXDrawDrawerConfig* const GDIconfig = static_cast<DirectXDrawDrawerConfig*const>(config);
        m_config = GDIconfig;
    }

    bool DirectXDrawDrawer::Initialize()
    {
        // Create version 7.0 DirectDraw object interface
        if (FAILED(DirectDrawCreateEx(nullptr, (void**)&lpdd7, IID_IDirectDraw7, nullptr)))
        {
            return false;
        }

        if (m_config->m_isFullScreen)
        {
            // Set the cooperation level for full screen
            if (FAILED(lpdd7->SetCooperativeLevel(m_config->m_hwnd, DDSCL_FULLSCREEN |
                DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
            {
                return false;
            }

            HRESULT result = lpdd7->SetDisplayMode(m_config->resolutionX, m_config->resolutionY, 32, 0, 0);
            if (result != DD_OK)
            {
                return false;
            }

            //DirectDraw surface description
            DDSURFACEDESC2 ddsd;
            memset(&ddsd, 0, sizeof(ddsd));
            ddsd.dwSize = sizeof(ddsd);
            ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
            ddsd.dwBackBufferCount = 1;
            ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

            if (FAILED(lpdd7->CreateSurface(&ddsd, &lpddsprimary, nullptr)))
            {
                return false;
            }

            ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;

            if (FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsback)))
            {
                return false;
            }

            //TODO Clipper
        }
        else
        {
            HRESULT result;
            // Set the cooperation level for windowed Direct Draw
            if (FAILED(lpdd7->SetCooperativeLevel(m_config->m_hwnd, DDSCL_NORMAL)))
            {

                return false;
            }

            //Without display mode in windowed

            //DirectDraw surface description
            DDSURFACEDESC2 ddsd;
            memset(&ddsd, 0, sizeof(ddsd));
            ddsd.dwSize = sizeof(ddsd);
            ddsd.dwFlags = DDSD_CAPS;
            ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

            if (FAILED(lpdd7->CreateSurface(&ddsd, &lpddsprimary, nullptr)))
            {
                return false;
            }

            // Pixel color format
            DDPIXELFORMAT ddPixelFormat;
            memset(&ddPixelFormat, 0, sizeof(ddPixelFormat));
            ddPixelFormat.dwSize = sizeof(ddPixelFormat);

            lpddsprimary->GetPixelFormat(&ddPixelFormat);
            int pixelFormat = ddPixelFormat.dwRGBBitCount;

            assert(pixelFormat == 32);
            
            // Window size, realwindowsRect will bigger than drawerRect
            RECT drawerRect = { 0, 0, (long)m_config->resolutionX, (long)m_config->resolutionY };
            RECT realwindowsRect = { 0, 0, (long)m_config->resolutionX, (long)m_config->resolutionY };
            AdjustWindowRectEx(&realwindowsRect, (DWORD)GetWindowLong(m_config->m_hwnd, GWL_STYLE), GetMenu(m_config->m_hwnd) != nullptr, (DWORD)GetWindowLong(m_config->m_hwnd, GWL_EXSTYLE));
            //(T=51,L=8,R=-8,B=-8)
            windowsOffsetRect = RectMinus(drawerRect, realwindowsRect);

           int X = (GetSystemMetrics(SM_CXSCREEN) - m_config->resolutionX) / 2;
           int Y = (GetSystemMetrics(SM_CYSCREEN) - m_config->resolutionY) / 2;
            MoveWindow(m_config->m_hwnd, X, Y, realwindowsRect.right - realwindowsRect.left, realwindowsRect.bottom - realwindowsRect.top, false);


            result = lpdd7->CreateClipper(0, &lpddClipper, nullptr);
            if (result != DD_OK)
            {
                return false;
            }

            result = lpddClipper->SetHWnd(0, m_config->m_hwnd);
            if (result != DD_OK)
            {
                return false;
            }

            result = lpddsprimary->SetClipper(lpddClipper);
            if (result != DD_OK)
            {
                return false;
            }
        }

        if (!m_config)
        {
            return false;
        }

        return true;

    }

    bool DirectXDrawDrawer::Shutdown()
    {
        if (lpddsprimary)
        {
            lpddsprimary->Release();
        }

        if (m_config->m_isFullScreen)
        {
            if (lpddsback)
            {
                lpddsback->Release();
            }
        }
        else 
        {
            if (lpddClipper)
            {
                lpddClipper->Release();
            }
        }

        if (lpdd7)
        {
            lpdd7->Release();
            lpdd7 = nullptr;
        }

        delete m_config;
        m_config = nullptr;

        //m_hdc = nullptr;

        return true;
    }

    bool DirectXDrawDrawer::Draw(IDrawerSurface* const drawerSurface)
    {
        //Make sure this is not executed again
        if (m_windowClosed)
        {
            return false;
        }

        assert(drawerSurface != nullptr);

        if (KEYDOWN(VK_ESCAPE))
        {
            PostMessage(m_config->m_hwnd, WM_CLOSE, 0, 0);
            m_windowClosed = true;
        }

        WindowSurface::DirectXDrawSurface* const directXDrawSurface = static_cast<WindowSurface::DirectXDrawSurface* const>(drawerSurface);
        
        RECT dest_rect;
        dest_rect.left = 0;
        dest_rect.top = 0;
        dest_rect.right = directXDrawSurface->GetWidth() ;
        dest_rect.bottom = directXDrawSurface->GetHeight();

        HRESULT result;

        if (m_config->m_isFullScreen)
        {
            result = lpddsback->Blt(nullptr, directXDrawSurface->GetDirectRawSurface(), nullptr, DDBLT_WAIT, nullptr);
            assert(result == DD_OK);

            while (FAILED(lpddsprimary->Flip(nullptr, DDFLIP_WAIT)));
        }
        else
        {
            RECT windowInScreen;
            GetWindowRect(m_config->m_hwnd, &windowInScreen);
            //windowInScreen.top += MathLab::abs(windowsOffsetRect.top);
            //windowInScreen.left += MathLab::abs(windowsOffsetRect.left);
            //windowInScreen.bottom -= MathLab::abs(windowsOffsetRect.top);
            //windowInScreen.right -= MathLab::abs(windowsOffsetRect.left);
            windowInScreen = RectAdd(windowInScreen, windowsOffsetRect);

            result = lpddsprimary->Blt(&windowInScreen, directXDrawSurface->GetDirectRawSurface(), nullptr, DDBLT_WAIT, nullptr);
            assert(result == DD_OK);

            DDSURFACEDESC2 ddsd;
            //Lock the back buffer
            memset(&ddsd, 0, sizeof(ddsd));
            ddsd.dwSize = sizeof(ddsd);

            if (FAILED(lpddsprimary->Lock(nullptr, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, nullptr)))
            {
                return false;
            }

            result = lpddsprimary->Unlock(nullptr);
            if (result != DD_OK)
            {
                return false;
            }
           
        }
        /*
        DDSURFACEDESC2 ddsd;
        //Lock the back buffer
        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);

        if (FAILED(lpddsback->Lock(nullptr, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, nullptr)))
        {
            return false;
        }

        UINT* backBuffer = (UINT*)ddsd.lpSurface;

        //Clean the back buffer buffer 
        if (ddsd.lPitch == m_config->resolutionX * sizeof(UINT))
        {
            //linear memory
            memset(backBuffer, 0, m_config->resolutionX * m_config->resolutionY * sizeof(UINT));
        }
        else
        {
            //non-linear memory
            UINT* destPtr = backBuffer;
            for (int i = 0; i < m_config->resolutionY; i++)
            {
                memset(destPtr, 0, m_config->resolutionX * sizeof(UINT));
                destPtr += ddsd.lPitch;
            }
        }

        result = lpddsback->Unlock(nullptr);
        if (result != DD_OK)
        {
            return false;
        }
        */
      
        //result = lpddsback->Blt(nullptr, nullptr, nullptr, DDBLT_COLORFILL, &ddbltfx);
        //if (result != DD_OK)
        //{
        //    return false;
        //}

        //result = lpddsprimary->BltFast(0, 0, directXDrawSurface->GetDirectRawSurface(), nullptr, DDBLTFAST_WAIT);
        //if (result != DD_OK)
        //{
        //    return false;
        //}




        ///////////////////TEST////////////////////////
        //DDBLTFX ddbltfx;
        //memset(&ddbltfx, 0, sizeof(ddbltfx));
        //ddbltfx.dwSize = sizeof(ddbltfx);
        //ddbltfx.dwFillColor = CrossPlatform::SColorRGBA(100, 20, 60).ToRGBALittleEndian();

        //RECT dest_rect;
        //dest_rect.left = 0;
        //dest_rect.top = 0;

        //dest_rect.right = directXDrawSurface->GetWidth() /2;
        //dest_rect.bottom = directXDrawSurface->GetHeight() /2;
        //
        //RECT src_rect;
        //src_rect.left = 0;
        //src_rect.top = 0;

        //src_rect.right = 100;
        //src_rect.bottom = 100;

        //HRESULT result;

        //DirectXDrawSurface surface(lpdd7,100,100, DDLOCK_SURFACEMEMORYPTR);
        //surface.lock();
        //surface.DrawPixel(50, 50, CrossPlatform::SColorRGBA(100, 20, 60));
        //surface.DrawPixel(51, 50, CrossPlatform::SColorRGBA(100, 20, 60));
        //surface.DrawPixel(52, 50, CrossPlatform::SColorRGBA(100, 20, 60));

        //printf("%d , ", surface.GetPixel(49, 50).ToRGBALittleEndian());
        //printf("%d , ", surface.GetPixel(50, 50).ToRGBALittleEndian());
        //printf("%d\n", surface.GetPixel(51, 50).ToRGBALittleEndian());
        //surface.Unlock();
        //LPDIRECTDRAWSURFACE7 surface7 = surface.GetDirectRawSurface();

        //result = lpddsback->Blt(nullptr, directXDrawSurface->GetDirectRawSurface(), nullptr, DDBLT_WAIT, nullptr);
        //if (result != DD_OK)
        //{
        //    return false;
        //}

        //while (FAILED(lpddsprimary->Flip(nullptr, DDFLIP_WAIT)));



        return true;
    }

    RECT RectAdd(const RECT& lhs, const RECT& rhs)
    {
        RECT result;
        result.top = lhs.top + rhs.top;
        result.left = lhs.left + rhs.left;
        result.right = lhs.right + rhs.right;
        result.bottom = lhs.bottom + rhs.bottom;
        return result;
    }

    RECT RectMinus(const RECT& lhs, const RECT& rhs)
    {
        RECT result;
        result.top = lhs.top - rhs.top;
        result.left = lhs.left - rhs.left;
        result.right = lhs.right - rhs.right;
        result.bottom = lhs.bottom - rhs.bottom;
        return result;
    }

}