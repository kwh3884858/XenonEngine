//  DirectXInput.h
//  XenonEngine
//
//  Created by whkong on 2021-4-28.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#pragma comment(lib, "dinput.lib")
#include <dinput.h>
#include "Mathlab/Vector2f.h"

namespace WindowInput {
    using MathLab::Vector2f;

    class DirectXInputConfig {
        HWND m_hwnd;
    };
    const unsigned char RightButton = MOUSE_RIGHT_BUTTON;
    const unsigned char LeftButton = MOUSE_LEFT_BUTTON;

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

        bool CALLBACK DirectInputEnumJoystick(LPCDIDEVICEINSTANCE lpddi, LPVOID guidPointer);

    private:
        LPDIRECTINPUT8 lpdi = nullptr;
        IDirectInputDevice8 lpdikey = nullptr;
        LPDIRECTINPUTDEVICE8 lpdiMouse = nullptr;
        LPDIRECTINPUTDEVICE8 lpdiJoystick = nullptr;
        HWND m_hwnd;
        unsigned char m_keyState[256];
        DIMOUSESTATE m_mouseState;
        DIJOYSTATE m_joystickState;
        char m_joyStickName[80];
        GUID m_joyStickGUID;

    };


}