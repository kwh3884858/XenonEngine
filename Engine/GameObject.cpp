#include "GameObject.h"

#include "Engine/Component/IComponent.h"
namespace XenonEngine
{

    GameObject::GameObject(const char* name /*= "XenonGameObject"*/) :
        m_name(name)
    {
    }

    GameObject* GameObject::Copy()const
    {
        GameObject* newGameObject = new GameObject("Untitled");
        String oldName = m_name;
        int pos = oldName.IndexOf("_");
        if (pos == -1)
        {
            oldName.Append("_0");
            newGameObject->m_name = oldName;
        }
        else
        {
            const char& order = oldName[pos + 1];
            int numOfGameObject = static_cast<int>(order);
            ++numOfGameObject;
            oldName = oldName.Substring(0, pos + 1);
            oldName.Append(static_cast<char>(numOfGameObject));
            newGameObject->m_name = oldName;
        }

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

    void GameObject::Start()
    {
        for (int i = 0; i < m_components.Count(); i++)
        {
            m_components[i]->Start();
        }
    }

	void GameObject::GameObjectUpdate()
	{
		for (int i = 0; i < m_components.Count(); i++)
		{
			m_components[i]->Update();
		}
		Update();
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

}