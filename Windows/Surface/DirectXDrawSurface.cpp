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

        UINT* backBuffer = (UINT*)m_directDrawSurfaceDescription.lpSurface;

        //Clean the back buffer buffer 
        if (m_directDrawSurfaceDescription.lPitch == m_width * sizeof(UINT))
        {
            //linear memory
            memset(backBuffer, 0, m_width * m_height * sizeof(UINT));
        }
        else
        {
            //non-linear memory
            UINT* destPtr = backBuffer;
            for (int i = 0; i < m_height; i++)
            {
                memset(destPtr, 0, m_width * sizeof(UINT));
                destPtr += m_directDrawSurfaceDescription.lPitch;
            }
        }
    }

    void DirectXDrawSurface::DrawPixel(unsigned int x, unsigned int y, SColorRGBA rgba)
    {
        assert(x < m_width  && y < m_height);
        UINT* buffer =static_cast<UINT*>( m_directDrawSurfaceDescription.lpSurface);
        buffer[y * m_width + x] = rgba.ToRGBALittleEndian();

    }

    void DirectXDrawSurface::DrawStraightLine(unsigned int xStart, unsigned int xEnd, unsigned int y, SColorRGBA rgba)
    {
        assert(xStart < m_width && xEnd < m_width  && y < m_height);
        UINT* buffer = static_cast<UINT*>(m_directDrawSurfaceDescription.lpSurface);
        memset(buffer + xStart, rgba.ToRGBALittleEndian(), xEnd - xStart + 1);
    }

    CrossPlatform::SColorRGBA DirectXDrawSurface::GetPixel(unsigned int x, unsigned int y)
    {
        assert(x < m_width  && y < m_height);
        UINT* buffer = static_cast<UINT*>(m_directDrawSurfaceDescription.lpSurface);
        return buffer[y * m_width + x];
    }

    void DirectXDrawSurface::Unlock()
    {
        HRESULT result = surface->Unlock(nullptr);
        assert(result == DD_OK);
    }

}