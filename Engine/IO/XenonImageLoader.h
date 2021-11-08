#pragma once
#include "Algorithms/String.h"
namespace CrossPlatform
{
    class XenonImage;
}

namespace XenonEngine
{
    class XenonImageLoader
    {
    public:
        CrossPlatform::XenonImage* LoadImage(const Algorithm::String& fileName)const;
    };
}