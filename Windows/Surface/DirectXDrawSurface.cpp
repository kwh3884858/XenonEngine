#include "Windows/Surface/DirectXDrawSurface.h"



namespace WindowSurface {


    DirectXDrawSurface::DirectXDrawSurface(LPDIRECTDRAW7 lpdd7, int width, int height, int mem0ry_flags)
    {
        m_width = width;
        m_height = height;

        DDSURFACEDESC2 ddsd;
        memset(&ddsd, 0, sizeof(ddsd));
        ddsd.dwSize = sizeof(ddsd);
        ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
        ddsd.dwWidth = width;
        ddsd.dwHeight = height;
        //Set surface
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem0ry_flags;

        //Create surface
        if (FAILED(lpdd7->CreateSurface(&ddsd, &surface, nullptr)))
        {
            return;
        }

        DDCOLORKEY colorKey;
        colorKey.dwColorSpaceLowValue = 0;
        colorKey.dwColorSpaceHighValue = 0;

        surface->SetColorKey(DDCKEY_SRCBLT, &colorKey);
    }

    DirectXDrawSurface::~DirectXDrawSurface()
    {
        if (surface)
        {
            surface->Release();
            surface = nullptr;
        }
    }

    void DirectXDrawSurface::lock()
    {
        surface->Lock(nullptr, &m_directDrawSurfaceDescription, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, nullptr);
    }

    void DirectXDrawSurface::DrawPixel(int x, int y, SColorRGBA rgba)
    {
        UINT* buffer = m_directDrawSurfaceDescription.lpSurface;
        buffer[i * m_width + j] = color;

    }

    void DirectXDrawSurface::Unlock()
    {
        surface->Unlock(nullptr);
    }

}