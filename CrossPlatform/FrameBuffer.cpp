#include "FrameBuffer.h"

namespace CrossPlatform
{
    float SColorRGBA::ToRGB()
    {
       return ( r | g << 8 | b << 16 );
    }

    float SColorRGBA::ToRGBA(){
        return (r | g << 8 | b << 16 | a << 24);
    }

    const int FramerBuffer::UNINITIALUZE_VALUE = -1;

    bool FramerBuffer::IsResolutionValid()
    {
        if (m_resolutionX == UNINITIALUZE_VALUE || m_resolutionY == UNINITIALUZE_VALUE)
        {
            return false;
        }
        return true;
    }

    bool FramerBuffer::IsBufferValid()
    {
        if (m_buffer == nullptr)
        {
            return false;
        }
        return true;
    }

    FramerBuffer::FramerBuffer()
    {
        m_buffer = nullptr;
        m_resolutionX = UNINITIALUZE_VALUE;
        m_resolutionY = UNINITIALUZE_VALUE;
    }

    bool FramerBuffer::CreateBuffer()
    {
        if (!IsResolutionValid()) { return; }

        if (m_buffer != nullptr)
        {
            ClearBuffer();
        }
    }


    bool FramerBuffer::ClearBuffer()
    {
        if (!IsBufferValid()) { return; }
        if (!IsResolutionValid()) { return; }
        
        delete m_buffer;
    }



}