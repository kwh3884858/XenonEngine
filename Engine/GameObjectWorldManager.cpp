#include "GameObjectWorldManager.h"
#include "Algorithms/String.h"
#include "Engine/GameObject.h"
#include "Engine/GameObjectWorld.h"
#include <filesystem>

#include "yaml-cpp/yaml.h"
#include "EngineManager.h"
#include "CrossPlatform/File/GameObjectWorldMeta.h"
#include "CrossPlatform/Converter/GameObjectWorldYamlConverter.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"
#include <fstream>


namespace XenonEngine
{
    using namespace std;
    using namespace std::filesystem;
    using namespace CrossPlatform;

    bool GameObjectWorldManager::Initialize()
    {
        if (!m_world)
        {
            CreateGameWorld("Empty World");
        }
        return true;
    }

    bool GameObjectWorldManager::Shutdown() 
    {
		RemoveGameWorld();
        return true;
    }

    GameObjectWorldManager::~GameObjectWorldManager()
    {
        assert(m_world == nullptr);
    }

    void GameObjectWorldManager::Update()
    {
		if (m_waitForAddedWorld != m_world)
		{
			RemoveGameWorld();
			m_world = m_waitForAddedWorld;
			m_world->Start();
		}

        if (!m_world)
        {
            return;
        }
		m_world->Update();
    }

    XenonEngine::GameObjectWorld*const GameObjectWorldManager::CreateGameWorld(const Algorithm::String& worldName)
    {
        GameObjectWorld* newWorld = new GameObjectWorld(worldName);
        SetCurrentWorld(newWorld);
        return newWorld;
    }

    GameObjectWorld*const GameObjectWorldManager::GetCurrentWorld() const
    {
        //assert(m_world != nullptr);
        return m_world;
    }

    void GameObjectWorldManager::SetCurrentWorld(GameObjectWorld* world)
    {
		RemoveGameWorld();
        m_waitForAddedWorld = world;
    }

	void GameObjectWorldManager::RemoveGameWorld()
	{
		if (m_world)
		{
			m_world->Destroy();
			delete m_world;
			m_world = nullptr;
		}
	}

}