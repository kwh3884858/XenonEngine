#include "InputSystem.h"
#include "CrossPlatform/Interface/IInput.h"

namespace XenonEngine
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

    Vector2f InputSystem::GetAxisRaw() const
    {
        return m_input->GetAxisRaw();
    }

    MathLab::Vector2f InputSystem::GetAxis() const
    {
        return m_input->GetAxis();
    }

    bool InputSystem::GetStickButton(unsigned buttonCode) const
    {
        return m_input->GetStickButton(buttonCode);
    }

}
