#pragma once

#include "CrossPlatform/Interface/IDrawer.h"

#include <windows.h>

namespace WindowDrawer {

    using CrossPlatform::IDrawer;
    using CrossPlatform::IDrawerConfig;
    using CrossPlatform::IDrawerSurface;
    using CrossPlatform::DrawerType;

    class WindowDGIDrawerConfig : public IDrawerConfig
    {
    public:
        virtual ~WindowDGIDrawerConfig()override{}

        unsigned int resolutionX;
        unsigned int resolutionY;
    };

    class WindowDGIDrawer : public IDrawer
    {
    public:
        WindowDGIDrawer():
            m_hdc(nullptr) ,
            m_config(nullptr)
        {
        };
        virtual ~WindowDGIDrawer()override;

        virtual void SetDrawerConfig(IDrawerConfig* const config) override;
        virtual bool Initialize() override;
        virtual bool Shutdown() override;

        virtual DrawerType GetType() const override {return DrawerType::GDI_Drawer; }
        virtual bool Draw(IDrawerSurface* const drawerSurface) override;

        //void SetFrameBufeer(FramerBufferHandler const frameBufferHandle);
        /*virtual const FramerBufferHandler GetFrameBuffer() const override { return m_frameBuffer; }*/
        void SetHDC(HDC hdc);
    private:
        /*FramerBufferHandler m_frameBuffer;*/
        HDC m_hdc;
        WindowDGIDrawerConfig* m_config;
    };
}

