//  DirectionLightComponent.h
//  XenonEngine
//
//  Created by whkong on 2021-10-18.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "LightComponent.h"
#include "MathLab/Vector3.h"
#include "MathLab/Vector4.h"
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

        DirectionLightComponent(GameObject* gameobject = nullptr) :
            LightComponent(gameobject, LightType::Direction) {}
        virtual ~DirectionLightComponent()override{}
        virtual ComponentType GetComponentType() const override { return m_type; };
        virtual IComponent* Copy(GameObject*const gameObject)const override;

        void SetConfig(const DirectionLightComponentConfig& config);
        //MathLab::Vector3f GetDirection()const;
        const MathLab::Vector3f& GetDirection();
        void SetDirection(const MathLab::Vector3f& direction) { m_direction = direction; }
        //CrossPlatform::SColorRGBA GetColor()const { return m_color; }
        CrossPlatform::SColorRGBA GetColor()const { return m_color.ToColor(); }
        const MathLab::Vector4f& GetRawColor()const { return m_color; }
        void SetRawColor(const MathLab::Vector4f& rawColor) { m_color = rawColor; }

        static ComponentType m_type;
    private:
        MathLab::Vector3f m_direction = MathLab::Vector3f(0, 0, 1);
        MathLab::Vector4f m_color;
    };

}