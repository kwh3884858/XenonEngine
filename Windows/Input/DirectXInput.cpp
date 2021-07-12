
#define INITGUI

#include "DirectXInput.h"
#include "CrossPlatform/XenonKey.h"
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
        m_hwnd = config->m_hwnd;
        m_mainInstance = config->m_mainInstance;
    }

    void DirectXInput::Initialize()
    {
        InitializeKeyboardCode();

        HRESULT result;
        result = DirectInput8Create(m_mainInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpdi, nullptr);
        assert(result == DI_OK);

        //Keybroad
        result = lpdi->CreateDevice(GUID_SysKeyboard, &lpdikey, nullptr);
        assert(result == DI_OK);

        result = lpdikey->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        assert(result == DI_OK);

        result = lpdikey->SetDataFormat(&c_dfDIKeyboard);
        assert(result == DI_OK);

        result = lpdikey->Acquire();
        assert(result == DI_OK);

        //Mouse
        result = lpdi->CreateDevice(GUID_SysMouse, &lpdiMouse, nullptr);
        assert(result == DI_OK);

        result = lpdiMouse->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        assert(result == DI_OK);

        result = lpdiMouse->SetDataFormat(&c_dfDIMouse);
        assert(result == DI_OK);

        result = lpdiMouse->Acquire();
        assert(result == DI_OK);

        //Joystick
        result = lpdi->EnumDevices(DI8DEVCLASS_GAMECTRL, DirectInputEnumJoystick, &m_joyStickGUID, DIEDFL_ATTACHEDONLY);
        assert(result == DI_OK);

        // If you get a error from here, please plug in a controller.
        result = lpdi->CreateDevice(m_joyStickGUID, &lpdiJoystick, nullptr);
        assert(result == DI_OK);

        result = lpdiJoystick->SetCooperativeLevel(m_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        assert(result == DI_OK);

        result = lpdiJoystick->SetDataFormat(&c_dfDIJoystick);
        assert(result == DI_OK);

        result = lpdiJoystick->Acquire();
        assert(result == DI_OK);
    }

    void DirectXInput::Update()
    {
        HRESULT result;
        result = lpdikey->GetDeviceState(sizeof(m_keyState), (LPVOID)m_keyState);
        assert(result == DI_OK);
        while (result == DIERR_INPUTLOST)
        {
            HRESULT state = lpdikey->Acquire();
            if (state != DI_OK)
            {
                break;
            }
        }

        result = lpdiMouse->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);
        assert(result == DI_OK);

        result = lpdiJoystick->Poll();
        //TODO: why is false?
        //assert(result == DI_OK);

        result = lpdiJoystick->GetDeviceState(sizeof(m_joystickState), (LPVOID)&m_joystickState);
        assert(result == DI_OK);

    }

    void DirectXInput::ShutDown()
    {
        if (lpdiJoystick)
        {
            lpdiJoystick->Unacquire();
        }
        if (lpdiJoystick)
        {
            lpdiJoystick->Release();
        }
        if (lpdiMouse)
        {
            lpdiMouse->Unacquire();
        }
        if (lpdiMouse)
        {
            lpdiMouse->Release();
        }
        if (lpdikey)
        {
            lpdikey->Unacquire();
        }
        if (lpdikey)
        {
            lpdikey->Release();
        }
        if (lpdi)
        {
            lpdi->Release();
        }
    }

    bool DirectXInput::GetKeyDown(unsigned char keyCode) const
    {
        assert(keyCode >= 0 && keyCode <= 255);
        return m_keyState[keyCode] & 0x80;
    }

    Vector2f DirectXInput::GetMouseMove() const
    {
        return Vector2f(m_mouseState.lX, m_mouseState.lY);
    }

    bool DirectXInput::GetMouseButton(unsigned char mouseCode) const
    {
        assert(mouseCode >= 0 && mouseCode <= 3);
        return m_mouseState.rgbButtons[mouseCode] & 0x80;
    }
 
    Vector2f DirectXInput::GetAxisRaw() const
    {
        return Vector2f(m_joystickState.lX, m_joystickState.lY);
    }

    Vector2f DirectXInput::GetAxis() const
    {
        return Vector2f((m_joystickState.lX - 32767.0f) / 32767.0f, (m_joystickState.lY - 32767.0f )/ 32767.0f);
    }

    bool DirectXInput::GetStickButton(unsigned buttonCode) const
    {
        assert(buttonCode >= 0 && buttonCode <= 31);
        return m_joystickState.rgbButtons[buttonCode] & 0x80;
    }

    void DirectXInput::InitializeKeyboardCode() const
    {
        CrossPlatform::XenonKey_ESCAPE           = DIK_ESCAPE          ;
        CrossPlatform::XenonKey_1                = DIK_1               ;
        CrossPlatform::XenonKey_2                = DIK_2               ;
        CrossPlatform::XenonKey_3                = DIK_3               ;
        CrossPlatform::XenonKey_4                = DIK_4               ;
        CrossPlatform::XenonKey_5                = DIK_5               ;
        CrossPlatform::XenonKey_6                = DIK_6               ;
        CrossPlatform::XenonKey_7                = DIK_7               ;
        CrossPlatform::XenonKey_8                = DIK_8               ;
        CrossPlatform::XenonKey_9                = DIK_9               ;
        CrossPlatform::XenonKey_0                = DIK_0               ;
        CrossPlatform::XenonKey_MINUS            = DIK_MINUS           ;    /* - on main keyboard */
        CrossPlatform::XenonKey_EQUALS           = DIK_EQUALS          ;
        CrossPlatform::XenonKey_BACK             = DIK_BACK            ;    /* backspace */
        CrossPlatform::XenonKey_TAB              = DIK_TAB             ;
        CrossPlatform::XenonKey_Q                = DIK_Q               ;
        CrossPlatform::XenonKey_W                = DIK_W               ;
        CrossPlatform::XenonKey_E                = DIK_E               ;
        CrossPlatform::XenonKey_R                = DIK_R               ;
        CrossPlatform::XenonKey_T                = DIK_T               ;
        CrossPlatform::XenonKey_Y                = DIK_Y               ;
        CrossPlatform::XenonKey_U                = DIK_U               ;
        CrossPlatform::XenonKey_I                = DIK_I               ;
        CrossPlatform::XenonKey_O                = DIK_O               ;
        CrossPlatform::XenonKey_P                = DIK_P               ;
        CrossPlatform::XenonKey_LBRACKET         = DIK_LBRACKET        ;
        CrossPlatform::XenonKey_RBRACKET         = DIK_RBRACKET        ;
        CrossPlatform::XenonKey_RETURN           = DIK_RETURN          ;    /* Enter on main keyboard */
        CrossPlatform::XenonKey_LCONTROL         = DIK_LCONTROL        ;
        CrossPlatform::XenonKey_A                = DIK_A               ;
        CrossPlatform::XenonKey_S                = DIK_S               ;
        CrossPlatform::XenonKey_D                = DIK_D               ;
        CrossPlatform::XenonKey_F                = DIK_F               ;
        CrossPlatform::XenonKey_G                = DIK_G               ;
        CrossPlatform::XenonKey_H                = DIK_H               ;
        CrossPlatform::XenonKey_J                = DIK_J               ;
        CrossPlatform::XenonKey_K                = DIK_K               ;
        CrossPlatform::XenonKey_L                = DIK_L               ;
        CrossPlatform::XenonKey_SEMICOLON        = DIK_SEMICOLON       ;
        CrossPlatform::XenonKey_APOSTROPHE       = DIK_APOSTROPHE      ;
        CrossPlatform::XenonKey_GRAVE            = DIK_GRAVE           ;    /* accent grave */
        CrossPlatform::XenonKey_LSHIFT           = DIK_LSHIFT          ;
        CrossPlatform::XenonKey_BACKSLASH        = DIK_BACKSLASH       ;
        CrossPlatform::XenonKey_Z                = DIK_Z               ;
        CrossPlatform::XenonKey_X                = DIK_X               ;
        CrossPlatform::XenonKey_C                = DIK_C               ;
        CrossPlatform::XenonKey_V                = DIK_V               ;
        CrossPlatform::XenonKey_B                = DIK_B               ;
        CrossPlatform::XenonKey_N                = DIK_N               ;
        CrossPlatform::XenonKey_M                = DIK_M               ;
        CrossPlatform::XenonKey_COMMA            = DIK_COMMA           ;
        CrossPlatform::XenonKey_PERIOD           = DIK_PERIOD          ;    /* . on main keyboard */
        CrossPlatform::XenonKey_SLASH            = DIK_SLASH           ;    /* / on main keyboard */
        CrossPlatform::XenonKey_RSHIFT           = DIK_RSHIFT          ;
        CrossPlatform::XenonKey_MULTIPLY         = DIK_MULTIPLY        ;    /* * on numeric keypad */
        CrossPlatform::XenonKey_LMENU            = DIK_LMENU           ;    /* left Alt */
        CrossPlatform::XenonKey_SPACE            = DIK_SPACE           ;
        CrossPlatform::XenonKey_CAPITAL          = DIK_CAPITAL         ;
        CrossPlatform::XenonKey_F1               = DIK_F1              ;
        CrossPlatform::XenonKey_F2               = DIK_F2              ;
        CrossPlatform::XenonKey_F3               = DIK_F3              ;
        CrossPlatform::XenonKey_F4               = DIK_F4              ;
        CrossPlatform::XenonKey_F5               = DIK_F5              ;
        CrossPlatform::XenonKey_F6               = DIK_F6              ;
        CrossPlatform::XenonKey_F7               = DIK_F7              ;
        CrossPlatform::XenonKey_F8               = DIK_F8              ;
        CrossPlatform::XenonKey_F9               = DIK_F9              ;
        CrossPlatform::XenonKey_F10              = DIK_F10             ;
        CrossPlatform::XenonKey_NUMLOCK          = DIK_NUMLOCK         ;
        CrossPlatform::XenonKey_SCROLL           = DIK_SCROLL          ;    /* Scroll Lock */
        CrossPlatform::XenonKey_NUMPAD7          = DIK_NUMPAD7         ;
        CrossPlatform::XenonKey_NUMPAD8          = DIK_NUMPAD8         ;
        CrossPlatform::XenonKey_NUMPAD9          = DIK_NUMPAD9         ;
        CrossPlatform::XenonKey_SUBTRACT         = DIK_SUBTRACT        ;    /* - on numeric keypad */
        CrossPlatform::XenonKey_NUMPAD4          = DIK_NUMPAD4         ;
        CrossPlatform::XenonKey_NUMPAD5          = DIK_NUMPAD5         ;
        CrossPlatform::XenonKey_NUMPAD6          = DIK_NUMPAD6         ;
        CrossPlatform::XenonKey_ADD              = DIK_ADD             ;    /* + on numeric keypad */
        CrossPlatform::XenonKey_NUMPAD1          = DIK_NUMPAD1         ;
        CrossPlatform::XenonKey_NUMPAD2          = DIK_NUMPAD2         ;
        CrossPlatform::XenonKey_NUMPAD3          = DIK_NUMPAD3         ;
        CrossPlatform::XenonKey_NUMPAD0          = DIK_NUMPAD0         ;
        CrossPlatform::XenonKey_DECIMAL          = DIK_DECIMAL         ;    /* . on numeric keypad */
        CrossPlatform::XenonKey_OEM_102          = DIK_OEM_102         ;    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
        CrossPlatform::XenonKey_F11              = DIK_F11             ;
        CrossPlatform::XenonKey_F12              = DIK_F12             ;
        CrossPlatform::XenonKey_F13              = DIK_F13             ;    /*                     (NEC PC98) */
        CrossPlatform::XenonKey_F14              = DIK_F14             ;    /*                     (NEC PC98) */
        CrossPlatform::XenonKey_F15              = DIK_F15             ;    /*                     (NEC PC98) */
        CrossPlatform::XenonKey_KANA             = DIK_KANA            ;    /* (Japanese keyboard)            */
        CrossPlatform::XenonKey_ABNT_C1          = DIK_ABNT_C1         ;    /* /? on Brazilian keyboard */
        CrossPlatform::XenonKey_CONVERT          = DIK_CONVERT         ;    /* (Japanese keyboard)            */
        CrossPlatform::XenonKey_NOCONVERT        = DIK_NOCONVERT       ;    /* (Japanese keyboard)            */
        CrossPlatform::XenonKey_YEN              = DIK_YEN             ;    /* (Japanese keyboard)            */
        CrossPlatform::XenonKey_ABNT_C2          = DIK_ABNT_C2         ;    /* Numpad . on Brazilian keyboard */
        CrossPlatform::XenonKey_NUMPADEQUALS     = DIK_NUMPADEQUALS    ;    /* = on numeric keypad (NEC PC98) */
        CrossPlatform::XenonKey_PREVTRACK        = DIK_PREVTRACK       ;    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
        CrossPlatform::XenonKey_AT               = DIK_AT              ;    /*                     (NEC PC98) */
        CrossPlatform::XenonKey_COLON            = DIK_COLON           ;    /*                     (NEC PC98) */
        CrossPlatform::XenonKey_UNDERLINE        = DIK_UNDERLINE       ;    /*                     (NEC PC98) */
        CrossPlatform::XenonKey_KANJI            = DIK_KANJI           ;    /* (Japanese keyboard)            */
        CrossPlatform::XenonKey_STOP             = DIK_STOP            ;    /*                     (NEC PC98) */
        CrossPlatform::XenonKey_AX               = DIK_AX              ;    /*                     (Japan AX) */
        CrossPlatform::XenonKey_UNLABELED        = DIK_UNLABELED       ;    /*                        (J3100) */
        CrossPlatform::XenonKey_NEXTTRACK        = DIK_NEXTTRACK       ;    /* Next Track */
        CrossPlatform::XenonKey_NUMPADENTER      = DIK_NUMPADENTER     ;    /* Enter on numeric keypad */
        CrossPlatform::XenonKey_RCONTROL         = DIK_RCONTROL        ;
        CrossPlatform::XenonKey_MUTE             = DIK_MUTE            ;    /* Mute */
        CrossPlatform::XenonKey_CALCULATOR       = DIK_CALCULATOR      ;    /* Calculator */
        CrossPlatform::XenonKey_PLAYPAUSE        = DIK_PLAYPAUSE       ;    /* Play / Pause */
        CrossPlatform::XenonKey_MEDIASTOP        = DIK_MEDIASTOP       ;    /* Media Stop */
        CrossPlatform::XenonKey_VOLUMEDOWN       = DIK_VOLUMEDOWN      ;    /* Volume - */
        CrossPlatform::XenonKey_VOLUMEUP         = DIK_VOLUMEUP        ;    /* Volume + */
        CrossPlatform::XenonKey_WEBHOME          = DIK_WEBHOME         ;    /* Web home */
        CrossPlatform::XenonKey_NUMPADCOMMA      = DIK_NUMPADCOMMA     ;    /* , on numeric keypad (NEC PC98) */
        CrossPlatform::XenonKey_DIVIDE           = DIK_DIVIDE          ;    /* / on numeric keypad */
        CrossPlatform::XenonKey_SYSRQ            = DIK_SYSRQ           ;
        CrossPlatform::XenonKey_RMENU            = DIK_RMENU           ;    /* right Alt */
        CrossPlatform::XenonKey_PAUSE            = DIK_PAUSE           ;    /* Pause */
        CrossPlatform::XenonKey_HOME             = DIK_HOME            ;    /* Home on arrow keypad */
        CrossPlatform::XenonKey_UP               = DIK_UP              ;    /* UpArrow on arrow keypad */
        CrossPlatform::XenonKey_PRIOR            = DIK_PRIOR           ;    /* PgUp on arrow keypad */
        CrossPlatform::XenonKey_LEFT             = DIK_LEFT            ;    /* LeftArrow on arrow keypad */
        CrossPlatform::XenonKey_RIGHT            = DIK_RIGHT           ;    /* RightArrow on arrow keypad */
        CrossPlatform::XenonKey_END              = DIK_END             ;    /* End on arrow keypad */
        CrossPlatform::XenonKey_DOWN             = DIK_DOWN            ;    /* DownArrow on arrow keypad */
        CrossPlatform::XenonKey_NEXT             = DIK_NEXT            ;    /* PgDn on arrow keypad */
        CrossPlatform::XenonKey_INSERT           = DIK_INSERT          ;    /* Insert on arrow keypad */
        CrossPlatform::XenonKey_DELETE           = DIK_DELETE          ;    /* Delete on arrow keypad */
        CrossPlatform::XenonKey_LWIN             = DIK_LWIN            ;    /* Left Windows key */
        CrossPlatform::XenonKey_RWIN             = DIK_RWIN            ;    /* Right Windows key */
        CrossPlatform::XenonKey_APPS             = DIK_APPS            ;    /* AppMenu key */
        CrossPlatform::XenonKey_POWER            = DIK_POWER           ;    /* System Power */
        CrossPlatform::XenonKey_SLEEP            = DIK_SLEEP           ;    /* System Sleep */
        CrossPlatform::XenonKey_WAKE             = DIK_WAKE            ;    /* System Wake */
        CrossPlatform::XenonKey_WEBSEARCH        = DIK_WEBSEARCH       ;    /* Web Search */
        CrossPlatform::XenonKey_WEBFAVORITES     = DIK_WEBFAVORITES    ;    /* Web Favorites */
        CrossPlatform::XenonKey_WEBREFRESH       = DIK_WEBREFRESH      ;    /* Web Refresh */
        CrossPlatform::XenonKey_WEBSTOP          = DIK_WEBSTOP         ;    /* Web Stop */
        CrossPlatform::XenonKey_WEBFORWARD       = DIK_WEBFORWARD      ;    /* Web Forward */
        CrossPlatform::XenonKey_WEBBACK          = DIK_WEBBACK         ;    /* Web Back */
        CrossPlatform::XenonKey_MYCOMPUTER       = DIK_MYCOMPUTER      ;    /* My Computer */
        CrossPlatform::XenonKey_MAIL             = DIK_MAIL            ;    /* Mail */
        CrossPlatform::XenonKey_MEDIASELECT      = DIK_MEDIASELECT     ;    /* Media Select */
    }

    char joystickName[80];
    BOOL CALLBACK DirectInputEnumJoystick(LPCDIDEVICEINSTANCE lpddi, LPVOID guidPointer)
    {
        *(GUID*)guidPointer = lpddi->guidInstance;
        strcpy(joystickName, (char*)lpddi->tszProductName);
        return (DIENUM_STOP);
    }

}