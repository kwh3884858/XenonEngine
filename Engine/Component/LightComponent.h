//  LightComponent.h
//  XenonEngine
//
//  Created by whkong on 2021-10-18.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
namespace XenonEngine
{
    class LightComponent :public IComponent
    {
    public:
        enum LightType
        {
            None,
            Direction,
            Point
        };
        LightComponent(GameObject* gameobject, LightType colliderType) :
            IComponent(gameobject),
            m_colliderType(colliderType)
        {}  
        virtual ~LightComponent()override {}
        virtual ComponentType GetComponentType() const override { return m_type; };
        LightType GetColliderType() const { return m_colliderType; }

        static ComponentType m_type;
    private:
        LightType m_colliderType = LightType::None;
    };
}