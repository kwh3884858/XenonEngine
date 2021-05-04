//  Rigidbody2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-3.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
#include "IComponent.h"
#include "MathLab/Vector2f.h"
#include "Algorithms/Vector.h"

namespace XenonPhysics
{
    class Physics2D;
    class Force2D;
}

namespace XenonEngine
{
    using MathLab::Vector2f;
    using Algorithm::Vector;
    class Rigidbody2D final : public IComponent
    {
    public:
        Rigidbody2D(GameObject* gameobject) :
            IComponent(ComponentType::Rigidbody2D, gameobject) {}
        virtual ~Rigidbody2D() override;

        bool AddForce(const Physics2D*const force);
        //Interfrated one time step
        bool FixedUpdate(double deltaTime);

    private:
        bool mIsStatic;
        bool mIsModified;
        float mMass;
        float mInertia;
        float mSpeed;

        Vector<Physics2D*> m_forces;
    }

}