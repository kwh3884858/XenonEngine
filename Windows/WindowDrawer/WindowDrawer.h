#pragma once


#include "IDrawer.h"

namespace WindowDrawer {

    using CrossPlatform::FramerBuffer;

    class WindowDrawer : public IDrawer
    {
    public:
        WindowDrawer(): m_frameBuffer(nullptr), m_hdc(nullptr) {};
        virtual ~WindowDrawer() override;

        virtual void Draw() override;
        virtual void SetFrameBufeer( FramerBufferHandler const frameBufferHandle) override;
        void SetHDC(HDC hdc);
    private:
        FramerBufferHandler m_frameBuffer;
        HDC m_hdc;
    };
}
