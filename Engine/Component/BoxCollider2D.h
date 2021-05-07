//  BoxBoxCollider2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "Collider2D.h"
#include "MathLab/Vector2f.h"
namespace XenonEngine
{
    using MathLab::Vector2f;

    class BoxCollider2DConfig 
    {
    public:
        bool m_isTrigger = false;
        Vector2f m_size;
    };

    class BoxCollider2D final :public Collider2D
    {
    public:
        BoxCollider2D(GameObject* gameobject) :
            Collider2D(gameobject, ColliderType::Box) {}
        virtual ~BoxCollider2D()override;

        void SetConfig(const BoxCollider2DConfig*const config);
        Vector2f GetSize()const { return m_size; }
        virtual float GetArea()const override;
        virtual float GetRadius()const override;
        
    private:
        Vector2f m_size;

    };

}