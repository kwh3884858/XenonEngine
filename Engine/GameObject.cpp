#include "GameObject.h"

#include "Engine/Component/IComponent.h"
namespace XenonEngine
{

    GameObject::GameObject(const char* name /*= "XenonGameObject"*/):
        m_name(name)
    {
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

    void GameObject::Update()
    {
        for (int i = 0; i < m_components.Count(); i++)
        {
            m_components[i]->Update();
        }
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