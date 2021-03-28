//  DirectXDrawDrawer.h
//  MacWindow
//
//  Created by whkong on 2021-3-15.
//  Copyright (c) 2018 whkong. All rights reserved.

#pragma once

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

#include "CrossPlatform/Interface/IDrawer.h"

#include <windows.h>
#include <ddraw.h>

namespace WindowDrawer {

    using CrossPlatform::IDrawer;
    using CrossPlatform::IDrawerConfig;
    using CrossPlatform::DrawerType;
    using CrossPlatform::IDrawerSurface;

    class DirectXDrawDrawerConfig : public IDrawerConfig
    {
    public:
        virtual ~DirectXDrawDrawerConfig()override{}

        unsigned int resolutionX;
        unsigned int resolutionY;
        HWND m_hwnd;
        bool m_isFullScreen;
    };

    class DirectXDrawDrawer : public IDrawer
    {
    public:
        DirectXDrawDrawer() {}
        virtual ~DirectXDrawDrawer()override;

        virtual void SetDrawerConfig(IDrawerConfig* const config) override;
        virtual bool Initialize() override;
        virtual bool Shutdown() override;

        virtual DrawerType GetType() const override { return DrawerType::DirectX_Draw_Drawer; }
        virtual bool Draw(IDrawerSurface* const drawerSurface) override;

        //void SetFrameBufeer(FramerBufferHandler const frameBufferHandle);
        //virtual const FramerBufferHandler GetFrameBuffer() const override{ return m_frameBuffer; }
        void SetHDC(HDC hdc);

        LPDIRECTDRAW7 GetDirectRaw() { return lpdd7; }
    private:
        //FramerBufferHandler m_frameBuffer;
        bool m_windowClosed = false;

        LPDIRECTDRAW7 lpdd7;
        //Interface pointer to hold primary surface
        LPDIRECTDRAWSURFACE7 lpddsprimary = nullptr;
        LPDIRECTDRAWSURFACE7 lpddsback = nullptr;
        LPDIRECTDRAWCLIPPER lpddClipper = nullptr;

        DirectXDrawDrawerConfig* m_config;
    };
}