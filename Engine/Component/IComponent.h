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
            None = 0,
            ComponentType_Transform2D,
			ComponentType_Transform3D,
            ComponentType_Mesh2D,
            ComponentType_Mesh3D,
            ComponentType_Shader,
            ComponentType_Collider2D,
            ComponentType_Rigidbody2D,
            ComponentType_Personality,
            ComponentType_Script,
            ComponentType_Input,
			ComponentType_Camera,
            ComponentType_Light,
            ComponentType_Count
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
        void SetGameobject(GameObject* object) { m_gameobject = object; }

        static IComponentType m_type;
		
		enum ObjectState
		{
			Enable,
			MarkForDelete
		};
		bool IsMarkForDelete() { return m_state == ObjectState::MarkForDelete; }
		void SetState(ObjectState state) { m_state = state; }

    protected:
		ObjectState m_state = ObjectState::Enable;
        GameObject* m_gameobject = nullptr;

    };

    __declspec(selectany) IComponent::IComponentType IComponent::m_type = IComponent::IComponentType::None;

    typedef IComponent::IComponentType ComponentType;
}
