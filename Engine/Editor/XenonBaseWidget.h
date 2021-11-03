#pragma once
//No Use, Ready for future
namespace XenonEngine {

    class XenonBaseWidget
    {
    public:
        XenonBaseWidget() {}
        virtual ~XenonBaseWidget() {}

        virtual void Update(const void* data = nullptr) = 0;

    protected:
    };

}