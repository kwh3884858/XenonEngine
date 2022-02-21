#include "WindowDGIDrawer.h"
#include "Windows/Surface/DrawerSurface.h"

namespace WindowDrawer {

    using WindowSurface::DrawerSurface;
    using CrossPlatform::SColorRGBA;

    WindowDGIDrawer::~WindowDGIDrawer()
    {

    }

    bool WindowDGIDrawer::Draw(IDrawerSurface* const drawerSurface)
    {
        //if (!m_frameBuffer)
        //{
        //    return false;
        //}

        //int x = m_frameBuffer->GetResolitionX();
        //int y = m_frameBuffer->GetResolitionY();

        //if (x <=0 || y <= 0)
        //{
        //    return false;
        //}
        if (!drawerSurface)
        {
            return false;
        }

        DrawerSurface* const surface = static_cast<DrawerSurface* const>(drawerSurface);
        SColorRGBA* buffer = surface->GetBuffer();
        unsigned int width = surface->GetWidth();
        unsigned int height = surface->GetHeight();

        for (unsigned int i = 0; i < width; i++)
        {
            for (unsigned int j = 0; j < height; j++)
            {
                SetPixel(m_hdc, i, j, buffer[j * width + i].ToRGBALittleEndian());
            }
        }

        return true;
    }

    //void WindowDGIDrawer::SetFrameBufeer( FramerBufferHandler const frameBufferHandle)
    //{
    //    m_frameBuffer = frameBufferHandle;
    //}

    void WindowDGIDrawer::SetHDC(HDC hdc)
    {
        m_hdc = hdc;
    }

    //bool WindowDGIDrawer::Initialize(unsigned int resolutionX, unsigned int resolutionY)
    //{
    //    m_frameBuffer = new FramerBuffer();
    //    m_frameBuffer->Initilize(resolutionX, resolutionY);

    //    return true;
    //}


    void WindowDGIDrawer::SetDrawerConfig(IDrawerConfig* const config)
    {
        if (!config)
        {
            return;
        }

        WindowDGIDrawerConfig* const GDIconfig = static_cast<WindowDGIDrawerConfig*const>(config);
        m_config = GDIconfig;
    }

    bool WindowDGIDrawer::Initialize()
    {
        if (!m_config)
        {
            return false;
        }

        return true;
    }


    bool WindowDGIDrawer::Shutdown()
    {
        delete m_config;
        m_config = nullptr;

        m_hdc = nullptr;

        return true;
    }
}
