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
            Transform,
            Polygon2D,
            Shader,
            Collider2D,
            Rigidbody2D,
            Script,
            Input
        };
        IComponent(IComponentType type, GameObject* gameobject) :
            m_type(type),
            m_gameobject(gameobject)
        {
        }
        virtual ~IComponent() {};

        IComponentType GetComponentType() { return m_type; }

        //DO NOT reference other component
        virtual bool Start() { return true; }
        virtual bool Update() { return true; }
        virtual bool Destroy() { return true; }

        GameObject* GetGameObject() { return m_gameobject; }

    protected:
        IComponentType m_type = IComponentType::None;
        GameObject* m_gameobject = nullptr;

    };

    typedef IComponent::IComponentType ComponentType;
}
