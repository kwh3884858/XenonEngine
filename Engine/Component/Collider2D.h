//  Collider2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "IComponent.h"
namespace XenonEngine
{
    class Collider2D :public IComponent
    {
    public:
        enum ColliderType {
            None,
            Circle,
            Box
        };

        Collider2D(GameObject* gameobject, ColliderType colliderType) :
            IComponent(ComponentType::Collider2D, gameobject),
            m_colliderType(colliderType)
        {}
        virtual ~Collider2D()override = 0;

        bool IsTrigger() const { return mIsTrigger; }
        ColliderType GetColliderType() const { return m_colliderType; }

        virtual float GetArea()const = 0;
        virtual float GetRadius()const = 0;
    protected:
        bool mIsModified = false;
        bool mIsTrigger = false;
        ColliderType m_colliderType = ColliderType::None;
    private:

    };

}


/* ////////////////For Easy Create New Component//////////////////
#include "IComponent.h"
namespace XenonEngine
{
    class Collider2D final :public IComponent
    {
    public:
        Collider2D(GameObject* gameobject) :
            IComponent(ComponentType::Collider2D, gameobject) {}
        virtual ~Collider2D()override;

    private:

    };

}
*/