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
            m_lightType(colliderType)
        {}  
        virtual ~LightComponent()override {}
        virtual ComponentType GetComponentType() const override { return m_type; };
        LightType GetLightType() const { return m_lightType; }
        virtual bool Start() override;
        virtual bool Update() override;
        virtual bool Destroy() override;

        static ComponentType m_type;
    private:
        LightType m_lightType = LightType::None;
    };
}