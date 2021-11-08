#include "XenonImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Library/stb/stb_image.h"
#include "CrossPlatform/XenonImage.h"

#include "Algorithms/String.h"
#include <string>

namespace XenonEngine
{
    CrossPlatform::XenonImage* XenonImageLoader::LoadImage(const Algorithm::String& fileName) const
    {
        std::string inputfile(fileName.Beign(), fileName.Count());
        int iw, ih, n;
        unsigned char *idata = stbi_load(inputfile.c_str(), &iw, &ih, &n, 0);
        CrossPlatform::XenonImage* image = new CrossPlatform::XenonImage(idata, ih, iw, n);
        return image;
    }
}


