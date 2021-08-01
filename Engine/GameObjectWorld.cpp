#include "GameObjectWorld.h"
#include "GameObject.h"
#include "Engine/Component/Render2D.h"
#include "Engine/Physics/Physics2D.h"

namespace XenonEngine
{
    using XenonPhysics::Physics2D;

    GameObjectWorld::GameObjectWorld(const Algorithm::String& worldName)
    {
        m_physics2D = new Physics2D;
    }

    GameObjectWorld::~GameObjectWorld()
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            m_worldObjects[i]->Destroy();
            delete m_worldObjects[i];
        }
        delete m_physics2D;
    }

    void GameObjectWorld::AddGameObject(GameObject* const gameobject)
    {
        GameObject* sameNameObject = GetGameObject(gameobject->GetName());
        if (sameNameObject == nullptr)
        {
            gameobject->Start();
            m_physics2D->AddGameObject(gameobject);
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
        ClearMarkForDelete();
        m_physics2D->FixedUpdate();
        RenderUpdate();
    }

    void GameObjectWorld::ClearMarkForDelete()
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            if (m_worldObjects[i]->IsMarkForDelete())
            {
                m_worldObjects[i]->Destroy();
                m_physics2D->RemoveGameObject(m_worldObjects[i]);
                m_worldObjects.Remove(i);
                i--;
            }
        }
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