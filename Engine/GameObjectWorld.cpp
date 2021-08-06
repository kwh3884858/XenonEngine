#include "GameObjectWorld.h"
#include "GameObject.h"
#include "Engine/Component/Render2D.h"
#include "Engine/Physics/Physics2D.h"
#include "Algorithms/StringSort.h"
namespace XenonEngine
{
    using XenonPhysics::Physics2D;
	using Algorithm::String;
	using Algorithm::MSD;
	using Algorithm::Vector;

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
        Vector<String> sameNameObjects = GetGameObjectNameList(gameobject->GetName());
		if (sameNameObjects.Count() > 0)
		{
			MSD(sameNameObjects);
			String largestOrderName = sameNameObjects[sameNameObjects.Count() - 1];
			int pos = largestOrderName.IndexOf("_");
			if (pos == -1)
			{
				largestOrderName.Append("_0");
				gameobject->SetName(largestOrderName);
			}
			else
			{
				const char& order = largestOrderName[pos + 1];
				int numOfGameObject = static_cast<int>(order);
				++numOfGameObject;
				largestOrderName = largestOrderName.Substring(0, pos + 1);
				largestOrderName.Append(static_cast<char>(numOfGameObject));
				gameobject->SetName(largestOrderName);
			}
		}

        gameobject->Start();
        m_physics2D->AddGameObject(gameobject);
        m_worldObjects.Add(gameobject);
    }

    GameObject* GameObjectWorld::GetGameObject(const Algorithm::String& gameObjectName) const
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            if (m_worldObjects[i]->GetName() == gameObjectName)
            {
                return m_worldObjects[i];
            }
        }
        return nullptr;
    }

	Vector<GameObject*> GameObjectWorld::GetGameObjectList(const Algorithm::String & gameObjectName) const
	{
		Vector<GameObject*> list;
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			if (m_worldObjects[i]->GetName().Find(gameObjectName))
			{
				list.Add( m_worldObjects[i]);
			}
		}
		return list;
	}

	Vector<String> GameObjectWorld::GetGameObjectNameList(const Algorithm::String & gameObjectName) const
	{
		Vector<String> list;
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			if (m_worldObjects[i]->GetName().Find(gameObjectName))
			{
				list.Add(m_worldObjects[i]->GetName());
			}
		}
		return list;
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