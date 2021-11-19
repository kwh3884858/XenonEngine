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
		for (int i = 0; i < m_components.Count(); i++)
		{
			delete m_components[i];
			m_components[i] = nullptr;
		}
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
			m_components[i]->Destroy();
		}
	}

	void GameObject::Start()
    {

    }

	void GameObject::Update()
	{

	}

    void GameObject::Destroy()
    {

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

}