//  Rigidbody2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-3.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
#include "MathLab/Vector2f.h"
namespace XenonEngine
{
    using MathLab::Vector2f;
        class Rigidbody2D final : public IComponent
    {
    public:
        Rigidbody2D(GameObject* gameobject) :
            IComponent(ComponentType::Rigidbody2D, gameobject) {}
        virtual ~Rigidbody2D() override;

    private:
        float mMass;
        float mInertia;
        float mSpeed;
        bool mIsStatic;
        bool mIsModified;


        float fx;
        float fy;
        float fz;
        float fvalue;
        bool mIsForces;
    }

}