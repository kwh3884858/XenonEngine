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
        Collider2D(GameObject* gameobject) :
            IComponent(ComponentType::Collider2D, gameobject) {}
        virtual ~Collider2D()override;

        bool IsTrigger();
    protected:
        bool mIsTrigger;
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