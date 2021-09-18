#pragma once
#ifdef _WIN32
#define  IS_PC_PLATFORM true
#endif
#if IS_PC_PLATFORM
#include <tchar.h>

#include "MainWindow.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    MainWindow window(hInstance);
    window.Initialize();
    window.Run();
    window.Shutdown();
    return 0;
}

#endif
