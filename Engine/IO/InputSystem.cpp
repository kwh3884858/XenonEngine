#include "InputSystem.h"
#include "CrossPlatform/Interface/IInput.h"

namespace XenonEnigne
{
    using MathLab::Vector2f;

    void InputSystem::SetFileReader(CrossPlatform::IInput*const input)
    {
        m_input = input;
    }

    bool InputSystem::Shutdown()
    {
        m_input = nullptr;
        return true;
    }

    bool InputSystem::GetKeyDown(unsigned char keyCode) const
    {
        return m_input->GetKeyDown(keyCode);
    }

    Vector2f InputSystem::GetMouseMove() const
    {
        return m_input->GetMouseMove();
    }

    bool InputSystem::GetMouseButton(unsigned char mouseCode) const
    {
        return m_input->GetMouseButton(mouseCode);
    }

}
