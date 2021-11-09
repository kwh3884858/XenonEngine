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

#include "CrossPlatform/Database.h"

void ThreadEditor( XenonEngine::EngineSyncData& syncData)
{
    XenonEngine::pGlobalSyncData = &syncData;
    if (CrossPlatform::Database::Get().engineConfig.m_isEditorLaunch)
    {
        ImGUIMain();
    }
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
    CrossPlatform::Database::Get().Initialize();
    XenonEngine::EngineSyncData globalSyncData;

    std::thread engineThread(ThreadEngine, hInstance, std::ref(globalSyncData));
    std::thread guiThread(ThreadEditor, std::ref(globalSyncData));

    engineThread.join();
    guiThread.join();
    CrossPlatform::Database::Get().Shutdown();
    return 0;
}
#endif
