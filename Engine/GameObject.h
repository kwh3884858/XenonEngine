//  GameObject.h
//  MacWindow
//
//  Created by whkong on 2021-3-15.
//  Copyright (c) 2018 whkong. All rights reserved.

#pragma once
#include "Algorithms/Vector.h"
#include "Algorithms/String.h"
#include "Engine/Component/IComponent.h"
#include <cassert>

namespace XenonEngine
{
    using Algorithm::Vector;
    using Algorithm::String;

    class GameObject
    {
    public:
        enum ObjectState 
        {
            Enable,
            MarkForDelete
        };
        GameObject(const String& name = "Untitled");
        virtual GameObject* Copy()const;
        virtual ~GameObject();

        template<typename T>
        void AddComponent(T* component);
        template<typename T>
        T* GetComponentPointer();
        template<typename T>
        const T* GetComponentPointer()const;
        template<typename T>
        T& GetComponent();
        template<typename T>
        const T& GetComponent()const;
        template<typename T>
        bool RemoveComponent(T* type);

        const String& GetName()const { return m_name; }
        void SetName(const String& name) {  m_name = name; }
        bool IsMarkForDelete() { return m_state == ObjectState::MarkForDelete; }
        void SetState(ObjectState state) { m_state = state; }

        int GetComponentCount()const { return m_components.Count(); }
        const IComponent* GetComponentByIndex(int index)const { assert(index >= 0 && index < m_components.Count()); return m_components[index]; }

		void GameObjectStart();
		void GameObjectUpdate();
        virtual void Start();
        virtual void Update();
        virtual void Destroy();
        virtual void OnTrigger(GameObject* gameobject) { return; }
    protected:
        
    private:
		void CheckName(String& name)const;
        String m_name;
        ObjectState m_state;
        Vector<IComponent*> m_components;
    };

    template<typename T>
    void GameObject::AddComponent(T * component)
    {
        m_components.Add(static_cast<IComponent*>(component));
    }

    template<typename T>
    T * GameObject::GetComponentPointer()
    {
        ComponentType componentType = T::m_type;

        for (int i = 0; i < m_components.Count(); i++)
        {
            ComponentType type = m_components[i]->GetComponentType();
            if (type == componentType)
            {
                return static_cast<T*>(m_components[i]);
            }
        }

        return nullptr;
    }

    template<typename T>
    const T* GameObject::GetComponentPointer() const
    {
        return const_cast<const T*>(static_cast<const GameObject&>(*this).GetComponentPointer<T>());
    }

    template<typename T>
    T& GameObject::GetComponent()
    {
        T* result = GetComponentPointer<T>();
        return *result;
    }

    template<typename T>
    const T& GameObject::GetComponent() const
    {
        const T* result = GetComponentPointer<T>();
        return *result;
    }

    template<typename T>
    bool GameObject::RemoveComponent(T* type)
    {
        for (int i = 0; i < m_components.Count(); i++)
        {
            if (m_components[i] == type)
            {
                return true;
            }
        }
        return false;
    }

}
