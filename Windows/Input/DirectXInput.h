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

namespace WindowInput {
    using MathLab::Vector2f;

    class DirectXInputConfig {
    public:
        HWND m_hwnd;
        HINSTANCE m_mainInstance;
    };
    const unsigned char RightButton = 0;
    const unsigned char LeftButton = 1;

    class DirectXInput
    {
    public:
        DirectXInput();
        ~DirectXInput();

        void SetConfig(const DirectXInputConfig* const config);
        void Initialize();
        void Update();
        void ShutDown();

        bool GetKeyDown(unsigned char keyCode)const;
        Vector2f GetMouseMove()const;
        bool GetMouseButton(unsigned char mouseCode)const;

    private:
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