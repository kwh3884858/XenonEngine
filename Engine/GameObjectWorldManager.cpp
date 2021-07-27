#include "GameObjectWorldManager.h"
#include "Algorithms/String.h"
#include "Engine/GameObject.h"
#include "Engine/GameObjectWorld.h"
namespace XenonEngine
{

    bool GameObjectWorldManager::Initialize()
    {
        return true;
    }

    bool GameObjectWorldManager::Shutdown()
    {
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

    //void GameObjectWorldManager::AddGameObject(GameObject* gameobject)
    //{
    //    if (m_currentWorld != nullptr)
    //    {
    //        m_currentWorld->AddGameObject(gameobject);
    //    }
    //    for (int i = 0; i < m_worlds.Count(); i++)
    //    {
    //    }
    //     m_worlds.Add(gameobject); 
    //}

    //XenonEngine::GameObject* GameObjectWorldManager::GetGameObject(const Algorithm::String& GameObjectName) const
    //{
    //    for (int i = 0; i < m_worlds.Count(); i++)
    //    {
    //        if (m_worlds[i]->GetName() == GameObjectName)
    //        {
    //            return m_worlds[i];
    //        }
    //    }
    //}

    void GameObjectWorldManager::Update()
    {
        assert(m_currentWorld != nullptr);
        m_currentWorld->Update();
    }

    XenonEngine::GameObjectWorld*const GameObjectWorldManager::CreateGameWorld(const Algorithm::String& worldName)
    {
        GameObjectWorld* newWorld = new GameObjectWorld(worldName);
        m_worlds.Add(newWorld);
        if (m_currentWorld == nullptr)
        {
            m_currentWorld = newWorld;
        }
        return newWorld;
    }

    GameObjectWorld*const GameObjectWorldManager::GetCurrentWorld() const
    {
        assert(m_currentWorld != nullptr);
        return m_currentWorld;
    }

}