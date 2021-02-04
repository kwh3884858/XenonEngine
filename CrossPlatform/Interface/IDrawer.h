#pragma once

#include "CrossPlatform/FrameBuffer.h"

typedef CrossPlatform::FramerBuffer* FramerBufferHandler;

namespace CrossPlatform {

    class IDrawer
    {
    public:
        virtual ~IDrawer() = default;
        virtual void Draw() = 0;
        //virtual FramerBufferHandler GetFrameBuffer() = delete;
    };
}
