#include "WindowDGIDrawer.h"
#include "CrossPlatform//FrameBuffer.h"

namespace WindowDrawer {

    WindowDGIDrawer::~WindowDGIDrawer()
    {

    }

    bool WindowDGIDrawer::Draw()
    {
        //if (!m_frameBuffer)
        //{
        //    return false;
        //}

        //int x = m_frameBuffer->GetResolitionX();
        //int y = m_frameBuffer->GetResolitionY();

        if (x <=0 || y <= 0)
        {
            return false;
        }

        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                SetPixel(m_hdc, i, j, (COLORREF)m_frameBuffer->GetColor(i, j));
            }
        }
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

        m_frameBuffer = new FramerBuffer();
        m_frameBuffer->Initilize(m_config->resolutionX, m_config->resolutionY);
        return true;
    }


    bool WindowDGIDrawer::Shutdown()
    {
        delete m_frameBuffer;
        m_frameBuffer = nullptr;

        delete m_config;
        m_config = nullptr;

        m_hdc = nullptr;

        return true;
    }
}
