//  PointLightComponent.h
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
    struct PointLightComponentConfig
    {
        CrossPlatform::SColorRGBA m_color;
        float m_kc;
        float m_kl;
    };

    class PointLightComponent final :public LightComponent
    {
    public:
        static const float PI;

        PointLightComponent(GameObject* gameobject) :
            LightComponent(gameobject, LightType::Point) {}
        virtual ~PointLightComponent()override {}
        virtual ComponentType GetComponentType() const override { return m_type; };
        virtual IComponent* Copy(GameObject*const gameObject)const override;

        void SetConfig(const PointLightComponentConfig& config);

        static ComponentType m_type;
    private:
        CrossPlatform::SColorRGBA m_color;
        float m_kc;
        float m_kl;
        // Point light: color * intensity / kc + kl * distance
    };

}