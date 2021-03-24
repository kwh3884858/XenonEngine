#include "Windows/Surface/DirectXDrawSurface.h"
//#include "CrossPlatform/SColorRGBA.h"
#include <assert.h>

namespace WindowSurface {

    DirectXDrawSurface::DirectXDrawSurface(LPDIRECTDRAW7 lpdd7,unsigned int width,unsigned int height, int mem0ry_flags)
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
        HRESULT result;
        result = lpdd7->CreateSurface(&ddsd, &surface, nullptr);
        assert(result == DD_OK);

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
        memset(&m_directDrawSurfaceDescription, 0, sizeof(m_directDrawSurfaceDescription));
        m_directDrawSurfaceDescription.dwSize = sizeof(m_directDrawSurfaceDescription);

        HRESULT result = surface->Lock(nullptr, &m_directDrawSurfaceDescription, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, nullptr);
        assert(result == DD_OK);
    }

    void DirectXDrawSurface::DrawPixel(unsigned int x, unsigned int y, SColorRGBA rgba)
    {
        UINT* buffer =static_cast<UINT*>( m_directDrawSurfaceDescription.lpSurface);
        buffer[y * m_width + x] = rgba.ToRGBALittleEndian();

    }

    CrossPlatform::SColorRGBA DirectXDrawSurface::GetPixel(unsigned int x, unsigned int y)
    {
        UINT* buffer = static_cast<UINT*>(m_directDrawSurfaceDescription.lpSurface);
        return buffer[y * m_width + x];
    }

    void DirectXDrawSurface::Unlock()
    {
        surface->Unlock(nullptr);
    }


}