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

        Rigidbody2D(GameObject* gameobject, bool isStatic, float mass, float inertia);
        virtual ~Rigidbody2D() override;

        bool AddForce(const Physics2D*const force);
        //One time step
        bool FixedUpdate(double deltaTime);

    private:
        //Aggregates forces acting on rigidbody
        void CalculateForcesAndMoments(double deltaTime);

        bool mIsStatic = false;
        bool mIsModified = false;

        float mMass;
        float mInertia;
        float mInertiaInverse;

        Vector2f mVelocity; //velocity in world space
        Vector2f mLocalVelocity; //velocity in model space 
        Vector2f mLocalAngularVelocity; //angular velocity in model space

        float mSpeed;

        Vector2f m_gravity; // Simulation gravity, default value is (0, mass * gravity acceleration)
        
        Vector2f m_forces;
        Vector2f m_moment;
    }

}