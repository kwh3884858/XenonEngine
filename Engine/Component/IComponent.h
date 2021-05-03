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
        enum ComponentType
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
        IComponent(ComponentType type, GameObject* gameobject) :
            m_type(type),
            m_gameobject(gameobject)
        {
        }
        virtual ~IComponent() = 0;

        ComponentType GetComponentType() { return m_type; }

        //DO NOT reference other component
        virtual bool Start() { return true; }
        virtual bool Update() { return true; }
        virtual bool Destroy() { return true; }

    protected:
        ComponentType m_type = ComponentType::None;
        GameObject* m_gameobject = nullptr;

    };
}
