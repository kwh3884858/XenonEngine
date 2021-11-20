#include "GameObjectWorld.h"
#include "GameObject.h"
#include "Engine/Component/Mesh2D.h"
#include "Engine/Physics/Physics2D.h"
#include "Algorithms/StringSort.h"

#include "Engine/Graphic/Graphic3D.h"
#include <cstdio> //for printf
#include "Engine/Component/Mesh3D.h"

namespace XenonEngine
{
    using XenonPhysics::Physics2D;
	using Algorithm::String;
	using Algorithm::MSD;
	using Algorithm::Vector;

    GameObjectWorld::GameObjectWorld(const Algorithm::String& worldName)
    {
        m_physics2D = new Physics2D;
        SetWorldName(worldName);
    }

    GameObjectWorld::GameObjectWorld(const GameObjectWorld& otherWorld): GameObjectWorld(otherWorld.GetWorldName())
    {
        const Algorithm::Vector<GameObject*>& objects = otherWorld.GetWorldObjects();
        for (int i = 0; i < objects.Count(); i++)
        {
            GameObject* newGo = objects[i]->Copy();
            AddGameObject(newGo);
        }
    }

    GameObjectWorld::~GameObjectWorld()
    {
        delete m_physics2D;
        m_physics2D = nullptr;
    }

    XenonEngine::GameObjectWorld* GameObjectWorld::Copy() const
    {
        GameObjectWorld* newObjectWorld = new GameObjectWorld(*this);
        return newObjectWorld;
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

        bool addedToPhysicsWorld = m_physics2D->AddGameObject(gameobject);
        if (!addedToPhysicsWorld)
        {
            printf("%s doesn`t have a physical component. [Warning]\n", gameobject->GetName().CString());
        }
        m_worldObjects.Add(gameobject);
    }

	void GameObjectWorld::RemoveGameObject(GameObject* const gameobject)
	{
		bool addedToPhysicsWorld = m_physics2D->RemoveGameObject(gameobject);
		m_worldObjects.Remove(gameobject);
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

	void GameObjectWorld::Start()
	{
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			m_worldObjects[i]->GameObjectStart();
			Mesh3D* mesh3D = m_worldObjects[i]->GetComponentPointer<Mesh3D>();
			if (mesh3D)
			{
				m_renderList.Add(m_worldObjects[i]);
			}
		}
	}

	void GameObjectWorld::Update()
    {
        ClearMarkForDelete();
        m_physics2D->FixedUpdate();
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			m_worldObjects[i]->GameObjectUpdate();
		}
        RenderUpdate();
    }

	void GameObjectWorld::Destroy()
	{
		for (int i = 0; i < m_worldObjects.Count(); i++)
		{
			DeleteGameObject(m_worldObjects[i]);
		}
	}

	void GameObjectWorld::ClearMarkForDelete()
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            if (m_worldObjects[i]->IsMarkForDelete())
            {
				DeleteGameObject(m_worldObjects[i]);
                m_worldObjects.Remove(i);
                i--;
            }
        }
    }

	void GameObjectWorld::DeleteGameObject(GameObject* gameobject)
	{
		m_physics2D->RemoveGameObject(gameobject);
		Mesh3D* mesh3D = gameobject->GetComponentPointer<Mesh3D>();
		if (mesh3D)
		{
			m_renderList.Remove(gameobject);
		}
		gameobject->GameObjectDestory();
		delete gameobject;
		gameobject = nullptr;
	}

	void GameObjectWorld::RenderUpdate()
    {
        for (int i = 0; i < m_worldObjects.Count(); i++)
        {
            Mesh2D* render2D = m_worldObjects[i]->GetComponentPointer<Mesh2D>();
            if (render2D)
            {
                render2D->Update();
            }
        }

        Graphic3D::Get().Update();
    }
}