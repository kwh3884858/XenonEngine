//  GameObject.h
//  MacWindow
//
//  Created by whkong on 2021-3-15.
//  Copyright (c) 2018 whkong. All rights reserved.

#pragma once
#include "Algorithms/Vector.h"
#include <cassert>
namespace XenonEngine
{
    using Algorithm::Vector;

    class IComponent;
    enum ComponentType :int;

    class GameObject
    {
    public:
        GameObject(const char* name = "XenonGameObject");
        virtual ~GameObject();

        template<typename T>
        void AddComponent(T* component);
        template<typename T>
        T* GetComponent(ComponentType type);
        template<typename T>
        bool RemoveComponent(T* type);

        const char*const GetName()const;

        virtual void Start();
        virtual void Update();
        virtual void Destroy();

    private:
        const char* m_name;
        Vector<IComponent*> m_components;
    };

    template<typename T>
    void GameObject::AddComponent(T * component)
    {
        m_components.add(static_cast<IComponent*>(component));
    }

    template<typename T>
    T * GameObject::GetComponent(ComponentType type)
    {
        for (size_t i = 0; i < m_components.Count(); i++)
        {
            if (m_components[i]->GetComponentType() == type)
            {
                return static_cast<T*>(m_components[i]);
            }
        }
        return nullptr;
    }

    template<typename T>
    bool GameObject::RemoveComponent(T* type)
    {
        for (size_t i = 0; i < m_components.Count(); i++)
        {
            if (m_components[i] == type)
            {
                return true;
            }
        }
        return false;
    }

}
