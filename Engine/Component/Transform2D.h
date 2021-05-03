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
    using MathLab::Vector2f
    class Transform2D final: public IComponent
    {
    public:
        Transform2D(GameObject* gameobject) :
            IComponent(ComponentType::Transform, gameobject){}
        virtual ~Transform2D()override;

        void SetPosition(Vector2f position) { m_position = position; }
        void SetRotation(Vector2f rotation) { m_rotation = rotation; }

    private:
        bool m_isModified = false;
        Vector2f m_position;
        Vector2f m_rotation;
        
    };
}