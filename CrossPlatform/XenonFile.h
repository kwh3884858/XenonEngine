#pragma once
#include "Algorithms/String.h"
namespace CrossPlatform
{
    //const int CL = 0x0D;  // CL, carriage return
    //const int LF = 0x0A;  // LF, line feed, will be removed

    struct XenonFile
    {
        Algorithm::String m_content = nullptr;
        unsigned int m_lineSize = 0;
    };

}