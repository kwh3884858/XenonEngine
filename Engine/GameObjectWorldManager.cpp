#include "GameObjectWorldManager.h"
#include "Algorithms/String.h"
#include "Engine/GameObject.h"
#include "Engine/GameObjectWorld.h"
#include <filesystem>

#include "yaml-cpp/yaml.h"
#include "EngineManager.h"
#include "CrossPlatform/File/WorldMeta.h"
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
        if (!m_currentWorld)
        {
            CreateGameWorld("Empty World");
        }
        return true;
    }

    bool GameObjectWorldManager::Shutdown() 
    {
		m_currentWorld->Destroy();
        m_currentWorld = nullptr;
        for (int i = 0; i < m_worlds.Count(); i++)
        {
            delete m_worlds[i];
        }
        return true;
    }

    GameObjectWorldManager::~GameObjectWorldManager()
    {
        assert(m_currentWorld == nullptr);
        assert(m_worlds.Count() == 0);
    }

    void GameObjectWorldManager::Update()
    {
        assert(m_currentWorld != nullptr);
        m_currentWorld->Update();
    }

    XenonEngine::GameObjectWorld*const GameObjectWorldManager::CreateGameWorld(const Algorithm::String& worldName)
    {
        GameObjectWorld* newWorld = new GameObjectWorld(worldName);
        AddGameWorld(newWorld);
        return newWorld;
    }

    GameObjectWorld*const GameObjectWorldManager::GetCurrentWorld() const
    {
        assert(m_currentWorld != nullptr);
        return m_currentWorld;
    }

    void GameObjectWorldManager::AddGameWorld(GameObjectWorld* world, bool isSetAsCurrentWorld /*= false*/)
    {
        m_worlds.Add(world);
        if (m_currentWorld == nullptr || isSetAsCurrentWorld)
        {
			if (m_currentWorld)
			{
				m_currentWorld->Destroy();
			}
            m_currentWorld = world;
			world->Start();
        }
    }

}