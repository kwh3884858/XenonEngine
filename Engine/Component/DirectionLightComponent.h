//  DirectionLightComponent.h
//  XenonEngine
//
//  Created by whkong on 2021-10-18.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "LightComponent.h"
#include "MathLab/Vector3.h"
#include "CrossPlatform/SColorRGBA.h"

namespace XenonEngine
{
    struct DirectionLightComponentConfig
    {
        MathLab::Vector3f m_direction;
        CrossPlatform::SColorRGBA m_color;
    };

    class DirectionLightComponent final :public LightComponent
    {
    public:
        static const float PI;

        DirectionLightComponent(GameObject* gameobject) :
            LightComponent(gameobject, LightType::Direction) {}
        virtual ~DirectionLightComponent()override{}
        virtual ComponentType GetComponentType() const override { return m_type; };
        virtual IComponent* Copy(GameObject*const gameObject)const override;

        void SetConfig(const DirectionLightComponentConfig& config);
        MathLab::Vector3f GetDirection();
        CrossPlatform::SColorRGBA GetColor()const { return m_color; }

        static ComponentType m_type;
    private:
        MathLab::Vector3f m_direction;
        CrossPlatform::SColorRGBA m_color;
    };

}