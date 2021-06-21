#pragma once
#include "Algorithms/String.h"
namespace CrossPlatform
{


    struct XenonFile
    {
        // CL, carriage return = 0x0D;
        static const int CL;

        // LF, line feed, will be removed = 0x0A;
        static const int LF;

        Algorithm::String m_content ;
        unsigned int m_lineSize = 0;
        Algorithm::String m_fileName;
    };

    __declspec(selectany) const int XenonFile::CL = '\r';

    __declspec(selectany) const int XenonFile::LF = '\n';

}