#include "GameObject.h"

#include "Engine/Component/IComponent.h"
namespace XenonEngine
{
	GameObject::GameObject(const String & name):
		m_name(name)
	{
	}

	GameObject* GameObject::Copy()const
    {
		GameObject* newGameObject = new GameObject(m_name);

        for (int i = 0; i < m_components.Count(); i++)
        {
            IComponent* comp = m_components[i]->Copy(newGameObject);
            newGameObject->AddComponent(comp);
        }
        return newGameObject;
    }

    GameObject::~GameObject()
    {

    }

    void GameObject::GameObjectStart()
    {
        Start();
        for (int i = 0; i < m_components.Count(); i++)
        {
            m_components[i]->Start();
        }
    }

    void GameObject::GameObjectUpdate()
	{
        Update();
		for (int i = 0; i < m_components.Count(); i++)
		{
			m_components[i]->Update();
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
        for (int i = 0; i < m_components.Count(); i++)
        {
            m_components[i]->Destroy();

            delete m_components[i];
            m_components[i] = nullptr;
        }
        m_components.Clear();
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