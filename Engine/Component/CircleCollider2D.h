//  CircleCollider2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "Collider2D.h"
namespace XenonEngine
{
    class CircleCollider2DConfig 
    {
        float m_radius;
    };

    class CircleCollider2D final :public Collider2D
    {
    public:
        CircleCollider2D(GameObject* gameobject) :
            Collider2D(gameobject, ColliderType::Circle) {}
        virtual ~CircleCollider2D()override;

        void SetConfig(const CircleCollider2DConfig*const config);
    private:
        float m_radius;
    };


}