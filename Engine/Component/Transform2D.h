//  Transform2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-3.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
#include "MathLab/Vector2.h"
namespace XenonEngine
{
    class Rigidbody2D;
    using MathLab::Vector2f;
    class Transform2D final: public IComponent
    {
    public:
        friend class Rigidbody2D;

        Transform2D(GameObject* gameobject = nullptr) :
            IComponent(gameobject){}
        virtual ~Transform2D()override;
        virtual ComponentType GetComponentType() const override { return m_type; };
        virtual IComponent* Copy(GameObject*const gameObject)const override;

        void AddPosition(const Vector2f& position) { m_position += position; }
        void SetPosition(const Vector2f& position) { m_position = position; }
        const Vector2f& GetPosition()const { return m_position; }

        float GetOrientation()const { return m_orientation; }
        void SetOrientation(float rotation) { m_orientation = rotation; }
        void AddRotation(float rotation) { m_orientation += rotation; }

        static ComponentType m_type;
    private:
        Vector2f m_position;
        float m_orientation = 0;
    };
}