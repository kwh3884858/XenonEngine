#pragma once

#include "FrameBuffer.h"

namespace CrossPlatform {
    class FramerBuffer;
}

typedef CrossPlatform::FramerBuffer* FramerBufferHandler;

class IDrawer
{
public:
    virtual ~IDrawer() = default;
    virtual void Draw(FramerBufferHandler frameBufferHandle) = delete;
    //virtual FramerBufferHandler GetFrameBuffer() = delete;
};
