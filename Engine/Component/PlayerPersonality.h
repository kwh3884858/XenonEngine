//  PlayerPersonality.h
//  XenonEngine
//
//  Created by whkong on 2021-7-12.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
namespace XenonEngine
{
    class PlayerPersonality final :public IComponent
    {
    public:
        static const float PI;

        PlayerPersonality(GameObject* gameobject) :
            IComponent(gameobject) {}
        virtual ~PlayerPersonality()override {}
        virtual ComponentType GetComponentType() const override { return m_type; };

        void SetVelocity(float velocity) { m_velocity = velocity; }
        const float GetVelocity()const { return m_velocity; }

        static ComponentType m_type;

    private:

        float m_velocity;
    };

    __declspec(selectany) ComponentType PlayerPersonality::m_type = ComponentType::ComponentType_Personality;


}