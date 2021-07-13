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

        void SetJumpForce(float force) { m_jumpForce = force; }
        const float GetJumpForce()const { return m_jumpForce; }

        static ComponentType m_type;

    private:

        float m_velocity = 0;
        float m_jumpForce = 0;
    };

    __declspec(selectany) ComponentType PlayerPersonality::m_type = ComponentType::ComponentType_Personality;


}