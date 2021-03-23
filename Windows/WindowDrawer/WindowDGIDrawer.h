
#pragma once


#include "CrossPlatform/Interface/IDrawer.h"

#include <windows.h>

namespace WindowDrawer {

    using CrossPlatform::FramerBuffer;
    using CrossPlatform::IDrawer;
    using CrossPlatform::IDrawerConfig;
    using CrossPlatform::DrawerType;

    class WindowDGIDrawerConfig : public IDrawerConfig
    {
    public:
        unsigned int resolutionX;
        unsigned int resolutionY;
    };

    class WindowDGIDrawer : public IDrawer
    {
    public:
        WindowDGIDrawer():
            m_frameBuffer(nullptr),
            m_hdc(nullptr) ,
            m_config(nullptr)
        {
            int i = 0;
            i++;
        };
        virtual ~WindowDGIDrawer();

        virtual void SetDrawerConfig(IDrawerConfig* const config) override;
        virtual bool Initialize() override;
        virtual bool Shutdown() override;

        virtual DrawerType GetType() const override {return DrawerType::GDI_Drawer; }
        virtual bool Draw() override;

        //void SetFrameBufeer(FramerBufferHandler const frameBufferHandle);
        /*virtual const FramerBufferHandler GetFrameBuffer() const override { return m_frameBuffer; }*/
        void SetHDC(HDC hdc);
    private:
        /*FramerBufferHandler m_frameBuffer;*/
        HDC m_hdc;
        WindowDGIDrawerConfig* m_config;
    };
}

