//  DirectXInput.h
//  XenonEngine
//
//  Created by whkong on 2021-4-28.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#pragma comment(lib, "dinput8.lib")
//#define INITGUID
//#include <objbase.h> //need this one for GUIDS
#include <dinput.h>
#include "Mathlab/Vector2f.h"
#include "CrossPlatform/Interface/IInput.h"

namespace WindowInput {
    using MathLab::Vector2f;

    class DirectXInputConfig {
    public:
        HWND m_hwnd;
        HINSTANCE m_mainInstance;
    };

    class DirectXInput:public CrossPlatform::IInput
    {
    public:
        DirectXInput();
        ~DirectXInput();

        void SetConfig(const DirectXInputConfig* const config);
        void Initialize();
        void Update();
        void ShutDown();

        virtual bool GetKeyDown(unsigned char keyCode)const override;
        virtual Vector2f GetMouseMove()const override;
        virtual bool GetMouseButton(unsigned char mouseCode)const override;
        virtual Vector2f GetAxisRaw()const override;
        virtual Vector2f GetAxis()const override;
        virtual bool GetStickButton(unsigned buttonCode)const override;

    private:
        void InitializeKeyboardCode()const;

        LPDIRECTINPUT8 lpdi = nullptr;
        LPDIRECTINPUTDEVICE8 lpdikey = nullptr;
        LPDIRECTINPUTDEVICE8 lpdiMouse = nullptr;
        LPDIRECTINPUTDEVICE8 lpdiJoystick = nullptr;
        HWND m_hwnd;
        HINSTANCE m_mainInstance;
        unsigned char m_keyState[256];
        DIMOUSESTATE m_mouseState;
        DIJOYSTATE m_joystickState;
        GUID m_joyStickGUID;

    };

    extern char joystickName[80];
    BOOL CALLBACK DirectInputEnumJoystick(LPCDIDEVICEINSTANCE lpddi, LPVOID guidPointer);
}