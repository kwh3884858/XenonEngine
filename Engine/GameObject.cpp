#include "GameObject.h"

#include "IComponent.h"
namespace Gameplay
{

    GameObject::GameObject(const char* name /*= "XenonGameObject"*/):
        m_name(name),
    {
        AddComponent(new SaintTransform(this));
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
        }
    }

}