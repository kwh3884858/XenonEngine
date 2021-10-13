#include "GameObjectWorld.h"
#include "GameObject.h"
#include "Engine/Component/Mesh2D.h"
#include "Engine/Physics/Physics2D.h"
#include "Algorithms/StringSort.h"

#include "Engine/Graphic/Graphic3D.h"
#include <cstdio> //for printf

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
        Vector<const String*> sameNameObjects = GetGameObjectNameList(gameobject->GetName());
		if (sameNameObjects.Count() > 0)
		{
			MSD(sameNameObjects);
			const String* largestOrderName = sameNameObjects[sameNameObjects.Count() - 1];
            String newName(*largestOrderName);
			int pos = largestOrderName->IndexOf("_");
			if (pos == -1)
			{
                newName.Append("_0");
				gameobject->SetName(newName);
			}
			else
			{
				const char& order = newName[pos + 1];
				int numOfGameObject = static_cast<int>(order);
				++numOfGameObject;
                newName = newName.Substring(0, pos + 1);
                newName.Append(static_cast<char>(numOfGameObject));
				gameobject->SetName(newName);
			}
		}

        gameobject->GameObjectStart();
        bool addedToPhysicsWorld = m_physics2D->AddGameObject(gameobject);
        if (!addedToPhysicsWorld)
        {
            char msg[200];
            gameobject->GetName().CString(msg);
            strcat(msg, " doesn`t have a physical component. [Warning]\n");
            printf(msg);
        }
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

    Vector<const String*> GameObjectWorld::GetGameObjectNameList(const Algorithm::String & gameObjectName) const
	{
        Vector<const String*> list ;
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			if (m_worldObjects[i]->GetName().Find(gameObjectName))
			{
                const String* name = &(m_worldObjects[i]->GetName());
				list.Add(name);
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
            Mesh2D* render2D = m_worldObjects[i]->GetComponent<Mesh2D>();
            if (render2D)
            {
                render2D->Update();
            }
        }

        Graphic3D::Get().Update();
    }

	void GameObjectWorld::ObjectUpdate()
	{
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			m_worldObjects[i]->GameObjectUpdate();
		}
	}

}