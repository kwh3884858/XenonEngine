#include "GameObject.h"

#include "IComponent.h"
namespace XenonEngine
{

    GameObject::GameObject(const char* name /*= "XenonGameObject"*/):
        m_name(name),
    {
    }

    GameObject::~GameObject()
    {

    }

    const char*const GameObject::GetName() const
    {
        return m_name;
    }

    void GameObject::Start()
    {
        for (size_t i = 0; i < m_components.count(); i++)
        {
            m_components[i]->Start();
        }
    }

    void GameObject::Update()
    {
        for (size_t i = 0; i < m_components.count(); i++)
        {
            m_components[i]->Update();
        }
    }

    void GameObject::Destroy()
    {
        for (size_t i = 0; i < m_components.count(); i++)
        {
            m_components[i]->Destroy();

            delete m_components[i];
            m_components[i] = nullptr;
        }
        m_components.Clear();
    }

}