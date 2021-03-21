#include "Windows/WindowDrawer/DirectXDrawDrawer.h"

//#include <windows.h>

namespace WindowDrawer {
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

        // Set the cooperation level for full screen
        if (FAILED(lpdd7->SetCooperativeLevel(m_config->m_hwnd, DDSCL_FULLSCREEN |
            DDSCL_ALLOWMODEX | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
        {
            return false;
        }
        /*
        // Set the cooperation level for windowed Direct Draw
        if (FAILED(lpdd7->SetCooperativeLevel(GetHwnd(),DDSCL_NORMAL)))
        {
            return false;
        }
        */
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

        if (!m_config)
        {
            return false;
        }

        m_frameBuffer = new FramerBuffer();
        m_frameBuffer->Initilize(m_config->resolutionX, m_config->resolutionY);
        return true;

    }

    bool DirectXDrawDrawer::Shutdown()
    {
        if (lpddsprimary)
        {
            lpddsprimary->Release();
        }

        if (lpdd7)
        {
            lpdd7->Release();
            lpdd7 = nullptr;
        }

        delete m_frameBuffer;
        m_frameBuffer = nullptr;

        delete m_config;
        m_config = nullptr;

        //m_hdc = nullptr;

        return true;
    }

    bool DirectXDrawDrawer::Draw()
    {
        //Make sure this is not executed again
        if (m_windowClosed)
        {
            return false;
        }

        if (KEYDOWN(VK_ESCAPE))
        {
            PostMessage(m_config->m_hwnd, WM_CLOSE, 0, 0);
            m_windowClosed = true;
        }

        DDSURFACEDESC2 ddsd;
        //Lock the back buffer
        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);

        if (FAILED(lpddsback->Lock(nullptr, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,nullptr)))
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

        for (int i = 0; i < m_config->resolutionY; i++)
        {
            for (int j = 0; j < m_config->resolutionX; j++)
            {
                long color = m_frameBuffer->GetColor(j, i);
                backBuffer[i * m_config->resolutionX + j] = color;
            }
        }

        if (FAILED(lpddsback->Unlock(nullptr)))
        {
            return false;
        }

        while (FAILED(lpddsprimary->Flip(nullptr, DDFLIP_WAIT)));

        return true;
    }

}