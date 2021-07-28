//  Polygon2D.h
//  XenonEngine
//
//  Created by whkong on 2021-7-12.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "IComponent.h"

namespace CrossPlatform {
    class Polygon2D;
}

namespace XenonEngine
{
    class Render2DConfig
    {
    public:
        const CrossPlatform::Polygon2D* m_polygon2D;
    };

    class Render2D final :public IComponent
    {
    public:
        static const float PI;

        Render2D(GameObject* gameobject) :
            IComponent(gameobject) {}
        virtual ~Render2D()override;
        virtual ComponentType GetComponentType() const override { return m_type; };
        virtual IComponent* Copy(GameObject*const gameObject)const override;

        void SetConfig(const Render2DConfig*const config);
        void Update()const;

        static ComponentType m_type;
    private:
        const CrossPlatform::Polygon2D* m_polygon2D;
    };


}