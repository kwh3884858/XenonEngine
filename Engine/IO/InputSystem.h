#pragma once
#include "CrossPlatform/XenonManager.h"
#include "MathLab/Vector2f.h"

namespace CrossPlatform
{
    class IInput;
}

namespace XenonEngine {
    class InputSystem :public CrossPlatform::XenonManager<InputSystem>
    {
    public:
        virtual bool Initialize() override { return true; }
        void SetFileReader(CrossPlatform::IInput*const input);
        virtual bool Shutdown() override;

        bool GetKeyDown(unsigned char keyCode)const;
        MathLab::Vector2f GetMouseMove()const;
        bool GetMouseButton(unsigned char mouseCode)const;
        MathLab::Vector2f GetAxisRaw()const;
        MathLab::Vector2f GetAxis()const;
        bool GetStickButton(unsigned buttonCode)const;
    private:
        CrossPlatform::IInput* m_input;
    };

}