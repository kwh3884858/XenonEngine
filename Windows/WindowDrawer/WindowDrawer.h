
#pragma once

#include "CrossPlatform/Interface/IDrawer.h"

#include <windows.h>

namespace WindowDrawer {
    using CrossPlatform::FramerBuffer;
    using CrossPlatform::IDrawer;

    class WindowDrawer : public IDrawer
    {
    public:
        WindowDrawer(): m_frameBuffer(nullptr), m_hdc(nullptr) {};
        virtual ~WindowDrawer() override;

        bool Initialize(unsigned int resolutionX, unsigned int resolutionY);
        bool Shutdown();
        virtual void Draw() override;
        //virtual void SetFrameBufeer(FramerBufferHandler const frameBufferHandle) override;
        void SetFrameBufeer(FramerBufferHandler const frameBufferHandle);
        const FramerBufferHandler GetFrameBuffer() const { return m_frameBuffer; }
        void SetHDC(HDC hdc);
    private:
        FramerBufferHandler m_frameBuffer;
        HDC m_hdc;
    };
}

