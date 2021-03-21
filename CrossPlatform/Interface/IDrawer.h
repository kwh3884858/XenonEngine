#pragma once

#include "CrossPlatform/FrameBuffer.h"

#include "CrossPlatform/Interface/DrawerType.h"
#include "CrossPlatform/Interface/IDrawerConfig.h"

typedef CrossPlatform::FramerBuffer* FramerBufferHandler;

namespace CrossPlatform {

    class IDrawer
    {
    public:
        virtual bool Initialize() = 0;
        virtual bool Shutdown() = 0;

        virtual DrawerType GetType() const = 0;
        virtual void SetDrawerConfig(IDrawerConfig* const config) = 0;
        virtual bool Draw() = 0;
        virtual const FramerBufferHandler GetFrameBuffer() const = 0;
    };
}
