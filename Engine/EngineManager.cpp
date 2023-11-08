#pragma once

#include "Engine/EngineManager.h"

#include "Engine/Graphic/Graphic3D.h"
#include "Engine/EngineSyncData.h"
namespace XenonEngine
{

    bool EngineManager::Initialize()
    {
		m_xenonEngineEditor.Initialize();
        m_worldManager.Initialize();
        m_fileDatabase.Initialize();
        (*pGlobalSyncData).Graphic3DSetter(&(Graphic3D::Get()));
        (*pGlobalSyncData).DatabaseSetter(&m_fileDatabase);
        return true;
    }

    bool EngineManager::Shutdown()
    {
        (*pGlobalSyncData).WorldManagerSetter(nullptr);
        (*pGlobalSyncData).Graphic3DSetter(nullptr);

        m_fileDatabase.Shutdown();
        m_worldManager.Shutdown();
		m_xenonEngineEditor.Shutdown();

        return true;
    }

    EngineManager::~EngineManager()
    {

    }

    void EngineManager::Update(long timeInterval)
{
		// Editor Part
		m_xenonEngineEditor.Update();

        m_worldManager.Update(timeInterval);
        (*pGlobalSyncData).WorldManagerSetter(&m_worldManager);
    }

}
