#include "GameObjectWorld.h"
#include "Engine/GameObject.h"

namespace XenonEngine
{

    bool GameObjectWorld::Initialize()
    {
        return true;
    }

    bool GameObjectWorld::Shutdown()
    {
        for (int i = 0; i < m_world.Count(); i++)
        {
            delete m_world[i];
        }
        return true;
    }

    GameObjectWorld::~GameObjectWorld()
    {

    }

    XenonEngine::GameObject* GameObjectWorld::GetGameObject(const Algorithm::String& GameObjectName) const
    {
        for (int i = 0; i < m_world.Count(); i++)
        {
            if (m_world[i]->GetName() == GameObjectName)
            {
                return m_world[i];
            }
        }
    }

}