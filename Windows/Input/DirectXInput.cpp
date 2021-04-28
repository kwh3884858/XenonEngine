
#define INITGUI

#include "DirectXInput.h"
#include <cassert>
namespace WindowInput {
    DirectXInput::DirectXInput()
    {
    }

    DirectXInput::~DirectXInput()
    {
    }

    void DirectXInput::SetConfig(const DirectXInputConfig* const config)
    {
        m_hwnd = config;
    }

    void DirectXInput::Initialize()
    {
        HRESULT result;
        result = DirectInput8Create(m_hwnd, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpdi, nullptr);
        assert(result == DI_OK);
       
        result = lpdi->CreateDevice(GUID_SysKeyboard, &lpdikey, nullptr);
        assert(result == DI_OK);

        result = lpdikey->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        assert(result == DI_OK);

        result = lpdikey->SetDataFormat(&c_dfDIKeyboard);
        assert(result == DI_OK);

    }

    void DirectXInput::Update()
    {
        HRESULT result;
        result = lpdikey->GetDeviceData(sizeof(m_keyState), (LPVOID)m_keyState);
        assert(result == DI_OK);
    }

    void DirectXInput::ShutDown()
    {
        if (lpdikey)
        {
            lpdikey->Release();
        }
        if (lpdi)
        {
            lpdi->Release();
        }
    }

}