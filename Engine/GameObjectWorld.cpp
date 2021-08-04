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
		if (sameNameObject != nullptr)
		{
			String oldName = gameobject->GetName();
			int pos = oldName.IndexOf("_");
			if (pos == -1)
			{
				oldName.Append("_0");
				gameobject->SetName(oldName);
			}
			else
			{
				const char& order = oldName[pos + 1];
				int numOfGameObject = static_cast<int>(order);
				++numOfGameObject;
				oldName = oldName.Substring(0, pos + 1);
				oldName.Append(static_cast<char>(numOfGameObject));
				gameobject->SetName(oldName);
			}
		}

        gameobject->Start();
        m_physics2D->AddGameObject(gameobject);
        m_worldObjects.Add(gameobject);
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
		ObjectUpdate();
        RenderUpdate();
    }

    void GameObjectWorld::ClearMarkForDelete()
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            if (m_worldObjects[i]->IsMarkForDelete())
            {
                m_physics2D->RemoveGameObject(m_worldObjects[i]);
				m_worldObjects[i]->Destroy();
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

	void GameObjectWorld::ObjectUpdate()
	{
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			m_worldObjects[i]->GameObjectUpdate();
		}
	}

}