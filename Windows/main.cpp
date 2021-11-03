#pragma once
#ifdef _WIN32
#define  IS_PC_PLATFORM true
#endif

#if IS_PC_PLATFORM
#include <tchar.h>
#include <thread>
#include <future>         // std::promise, std::future

#include "MainWindow.h"
#include "Windows/Editor/EditorGUI.h"
#include "Engine/EngineSyncData.h"

void ThreadEditor( XenonEngine::EngineSyncData& syncData)
{
    XenonEngine::pGlobalSyncData = &syncData;
    ImGUIMain();
}

void ThreadEngine(HINSTANCE hInstance,  XenonEngine::EngineSyncData& syncData)
{
    XenonEngine::pGlobalSyncData = &syncData;
    MainWindow window(hInstance);
    window.Initialize();
    window.Run();
    window.Shutdown();
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    XenonEngine::EngineSyncData globalSyncData;

    std::thread engineThread(ThreadEngine, hInstance, std::ref(globalSyncData));
    std::thread guiThread(ThreadEditor, std::ref(globalSyncData));

    engineThread.join();
    guiThread.join();

    return 0;
}
#endif
