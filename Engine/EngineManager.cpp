#pragma once

#include "Engine/EngineManager.h"

#include "Engine/Graphic/Graphic3D.h"
#include "Engine/EngineSyncData.h"
namespace XenonEngine
{

    bool EngineManager::Initialize()
    {
        m_worldManager.Initialize();
        m_fileDatabase.Initialize();
        (*pGlobalSyncData).Graphic3DSetter(&(Graphic3D::Get()));
        (*pGlobalSyncData).FolderSetter(m_fileDatabase.GetDataRoot());

    }

    bool EngineManager::Shutdown()
    {
        (*pGlobalSyncData).WorldSetter(nullptr);
        (*pGlobalSyncData).Graphic3DSetter(nullptr);

        m_fileDatabase.Shutdown();
        m_worldManager.Shutdown();
    }

    EngineManager::~EngineManager()
    {

    }

    void EngineManager::Update()
    {
        m_worldManager.Update();
        (*pGlobalSyncData).WorldSetter(m_worldManager.GetCurrentWorld());

    }

}
