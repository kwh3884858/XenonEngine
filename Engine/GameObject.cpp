#include "GameObject.h"

#include "Engine/Component/IComponent.h"
#include <utility>

namespace XenonEngine
{
	GameObject::GameObject(const String & name):
		m_name(name),
        m_state(ObjectState::Enable)
	{
	}

	GameObject::GameObject(const GameObject& that):GameObject(that.m_name)
	{
		m_state = that.m_state;
		for (int i = 0; i < that.m_components.Count(); i++)
		{
			IComponent* comp = that.m_components[i]->Copy(this);
			AddComponent(comp);
		}
	}

	GameObject::GameObject(GameObject&& that)
	{
		m_name = that.m_name;
		m_state = that.m_state;
		m_components = std::move(that.m_components);
	}

	GameObject* GameObject::Copy()const
    {
		GameObject* newGameObject = new GameObject(*this);
		return newGameObject;
    }

    GameObject::~GameObject()
    {
		m_components.Clear();
    }

	const XenonEngine::GameObject& GameObject::operator=(const GameObject& rhs)
	{
		if (this == &rhs)
			return *this;

		m_name = rhs.m_name;
		m_state = rhs.m_state;
		for (int i = 0; i < rhs.m_components.Count(); i++)
		{
			IComponent* comp = rhs.m_components[i]->Copy(this);
			AddComponent(comp);
		}

		return *this;
	}

	const XenonEngine::GameObject& GameObject::operator=(GameObject&& rhs)
	{
		if (this == &rhs)
			return *this;

		m_name = rhs.m_name;
		m_state = rhs.m_state;
		m_components = std::move(rhs.m_components);

		return *this;
	}

	void GameObject::GameObjectStart()
    {
        Start();
        //for (int i = 0; i < m_components.Count(); i++)
        //{
        //    assert(m_components[i] != nullptr);
        //    m_components[i]->Start();
        //}
		for (IComponent*& comp : m_components)
		{
			assert(comp != nullptr);
			comp->Start();
		}
    }

    void GameObject::GameObjectUpdate()
	{
		ClearMarkForDelete();
        Update();
		//for (int i = 0; i < m_components.Count(); i++)
		//{
  //          assert(m_components[i] != nullptr);
		//	m_components[i]->Update();
		//}
		for (IComponent*& comp : m_components)
		{
			assert(comp != nullptr);
			comp->Update();
		}
	}

	void GameObject::GameObjectDestory()
	{
		Destroy();
		//for (int i = 0; i < m_components.Count(); i++)
		//{
		//	DeleteComponent(m_components[i]);
		//}
		for (IComponent*& comp : m_components)
		{
			DeleteComponent(comp); 
		}
	}

	void GameObject::OnTrigger(GameObject* gameobject)
	{
		gameobject; 
		return;
	}

	void GameObject::CheckName(String& name) const
	{
		int pos = name.IndexOf("_");
		if (pos == -1)
		{
			name.Append("_0");
		}
		else
		{
			const char& order = name[pos + 1];
			int numOfGameObject = static_cast<int>(order);
			++numOfGameObject;
			name = name.Substring(0, pos + 1);
			name.Append(static_cast<char>(numOfGameObject));
		}
	}

	void GameObject::ClearMarkForDelete()
	{
		for (int i = 0; i < m_components.Count(); i++)
		{
			if (m_components[i]->IsMarkForDelete())
			{
				DeleteComponent(m_components[i]);
				m_components.Remove(i);
				i--;
			}
		}
	}

	void GameObject::DeleteComponent(IComponent* component)
	{
		component->Destroy();
		delete component;
		component = nullptr;
	}

}