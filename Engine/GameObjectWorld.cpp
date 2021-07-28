#include "GameObjectWorld.h"
#include "GameObject.h"
#include "Engine/Component/Render2D.h"

namespace XenonEngine
{
    GameObjectWorld::GameObjectWorld(const Algorithm::String& worldName)
    {

    }

    GameObjectWorld::~GameObjectWorld()
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            m_worldObjects[i]->Destroy();
            delete m_worldObjects[i];
        }
    }

    void GameObjectWorld::AddGameObject(GameObject* const gameobject)
    {
        GameObject* sameNameObject = GetGameObject(gameobject->GetName());
        if (sameNameObject == nullptr)
        {
            m_worldObjects.Add(gameobject);
        }
        else
        {
            assert(true == false);
        }
         
    }

    GameObject* GameObjectWorld::GetGameObject(const Algorithm::String& GameObjectName) const
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            if (m_worldObjects[i]->GetName() == GameObjectName)
            {
                return m_worldObjects[i];
            }
        }
        return nullptr;
    }

    void GameObjectWorld::Update()
    {
        RenderUpdate();
    }

    void GameObjectWorld::RenderUpdate()
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            Render2D* render2D = m_worldObjects[i]->GetComponent<Render2D>();
            render2D->Update();
        }
    }

}