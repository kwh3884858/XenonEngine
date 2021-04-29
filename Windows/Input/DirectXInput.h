//  DirectXInput.h
//  XenonEngine
//
//  Created by whkong on 2021-4-28.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#pragma comment(lib, "dinput.lib")
#include <dinput.h>

namespace WindowInput {

    class DirectXInputConfig {
        HWND m_hwnd;
    };

    class DirectXInput
    {
    public:
        DirectXInput();
        ~DirectXInput();

        void SetConfig(const DirectXInputConfig* const config);
        void Initialize();
        void Update();
        void ShutDown();

        bool IsKeyDown(unsigned char keyCode)const;
    private:
        LPDIRECTINPUT8 lpdi = nullptr;
        IDirectInputDevice8 lpdikey = nullptr;
        LPDIRECTINPUTDEVICE8 lpdiMouse = nullptr;
        HWND m_hwnd;
        unsigned char m_keyState[256];
        DIMOUSESTATE m_mouseState;
    };


}