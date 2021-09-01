//  XenonIComponent.h
//  XenonEngine
//
//  Created by whkong on 2021-5-3.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
namespace XenonEngine
{
    class GameObject;

    class IComponent
    {
    public:
        enum IComponentType :int
        {
            None,
            ComponentType_Transform2D,
			ComponentType_Transform3D,
            ComponentType_Mesh2D,
            ComponentType_Shader,
            ComponentType_Collider2D,
            ComponentType_Rigidbody2D,
            ComponentType_Personality,
            ComponentType_Script,
            ComponentType_Input
        };
        IComponent( GameObject* gameobject) :
            m_gameobject(gameobject)
        {
        }
        virtual ~IComponent() {};

        virtual IComponentType GetComponentType() const = 0;

        //DO NOT reference other component
        virtual IComponent* Copy(GameObject*const gameObject) const = 0;
        virtual bool Start() { return true; }
        virtual bool Update() { return true; }
        virtual bool Destroy() { return true; }

        GameObject* GetGameObject()const { return m_gameobject; }

        static IComponentType m_type;
    protected:
        
        GameObject* m_gameobject = nullptr;

    };

    __declspec(selectany) IComponent::IComponentType IComponent::m_type = IComponent::IComponentType::None;

    typedef IComponent::IComponentType ComponentType;
}
