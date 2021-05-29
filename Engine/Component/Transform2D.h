//  Transform2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-3.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
#include "MathLab/Vector2f.h"
namespace XenonEngine
{
    class Rigidbody2D;
    using MathLab::Vector2f;
    class Transform2D final: public IComponent
    {
    public:
        friend class Rigidbody2D;

        Transform2D(GameObject* gameobject) :
            IComponent(ComponentType::Transform, gameobject){}
        virtual ~Transform2D()override;

        //void SetPosition(Vector2f position) { m_position = position; }
        //void SetRotation(float rotation) { m_orientation = rotation; }
        void AddPosition(const Vector2f& position) { m_position += position; }

        float GetOrientation()const { return m_orientation; }
        void AddRotation(float rotation) { m_orientation += rotation; }
    private:
        bool m_isModified = false;
        Vector2f m_position;
        float m_orientation;
    };
}