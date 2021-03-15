//  DirectXDrawDrawer.h
//  MacWindow
//
//  Created by whkong on 2021-3-15.
//  Copyright (c) 2018 whkong. All rights reserved.

#pragma once

#pragma comment(lib, "ddraw.lib")

#include "CrossPlatform/Interface/IDrawer.h"
#include <ddraw.h>

namespace WindowDrawer {
    using CrossPlatform::FramerBuffer;
    using CrossPlatform::IDrawer;
    using CrossPlatform::IDrawerConfig;
    using CrossPlatform::DrawerType;

    class DirectXDrawDrawerConfig : public IDrawerConfig
    {
    public:
        unsigned int resolutionX;
        unsigned int resolutionY;
    };

    class DirectXDrawDrawer : public IDrawer
    {
    public:
        DirectXDrawDrawer() : m_frameBuffer(nullptr), m_hdc(nullptr) {
            int i = 0;
            i++;
        };
        virtual ~DirectXDrawDrawer() override;

        bool Initialize(unsigned int resolutionX, unsigned int resolutionY);
        bool Shutdown();
        virtual void Draw() override;
        //virtual void SetFrameBufeer(FramerBufferHandler const frameBufferHandle) override;
        void SetFrameBufeer(FramerBufferHandler const frameBufferHandle);
        const FramerBufferHandler GetFrameBuffer() const { return m_frameBuffer; }
        void SetHDC(HDC hdc);
    private:
        FramerBufferHandler m_frameBuffer;
    };
}