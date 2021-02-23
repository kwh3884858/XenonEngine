#include "WindowDrawer.h"
#include "CrossPlatform//FrameBuffer.h"

namespace WindowDrawer {

    WindowDrawer::~WindowDrawer()
    {
        m_frameBuffer = nullptr;
        m_hdc = nullptr;
    }

    void WindowDrawer::Draw()
    {
        if (!m_frameBuffer)
        {
            return;
        }

        int x = m_frameBuffer->GetResolitionX();
        int y = m_frameBuffer->GetResolitionY();

        if (x <=0 || y <= 0)
        {
            return;
        }

        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                SetPixel(m_hdc, i, j, (COLORREF)m_frameBuffer->GetColor(i, j));
            }
        }
    }

    void WindowDrawer::SetFrameBufeer( FramerBufferHandler const frameBufferHandle)
    {
        m_frameBuffer = frameBufferHandle;
    }

    void WindowDrawer::SetHDC(HDC hdc)
    {
        m_hdc = hdc;
    }

    bool WindowDrawer::Initialize(unsigned int resolutionX, unsigned int resolutionY)
    {
        m_frameBuffer = new FramerBuffer();
        m_frameBuffer->Initilize(resolutionX, resolutionY);

        return true;
    }

    bool WindowDrawer::Shutdown()
    {
        delete m_frameBuffer;
        return true;
    }

}
