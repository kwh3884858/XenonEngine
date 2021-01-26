#pragma once

#include "FrameBuffer.h"


class IDrawer
{
public:
    IDrawer() = default;
    virtual ~IDrawer() = default;

    virtual FramerBufferHandler GetFrameBuffer() = delete;
};
