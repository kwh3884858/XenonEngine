#include "GameObject.h"

#include "Engine/Component/IComponent.h"
namespace XenonEngine
{
	GameObject::GameObject(const String & name):
		m_name(name),
        m_state(ObjectState::Enable)
	{
	}

	GameObject::GameObject(const GameObject& that):GameObject(that.m_name)
	{
		for (int i = 0; i < that.m_components.Count(); i++)
		{
			IComponent* comp = that.m_components[i]->Copy(this);
			AddComponent(comp);
		}
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

    void GameObject::GameObjectStart()
    {
        Start();
        for (int i = 0; i < m_components.Count(); i++)
        {
            assert(m_components[i] != nullptr);
            m_components[i]->Start();
        }
    }

    void GameObject::GameObjectUpdate()
	{
		ClearMarkForDelete();
        Update();
		for (int i = 0; i < m_components.Count(); i++)
		{
            assert(m_components[i] != nullptr);
			m_components[i]->Update();
		}
	}

	void GameObject::GameObjectDestory()
	{
		Destroy();
		for (int i = 0; i < m_components.Count(); i++)
		{
			DeleteComponent(m_components[i]);
		}
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