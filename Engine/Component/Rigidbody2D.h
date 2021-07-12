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
        virtual ComponentType GetComponentType() const override { return m_type; };

        bool FixedUpdate(float deltaTime);     //One time step

        bool AddForce(const XenonPhysics::Force2D& force);

        Vector2f GetVelocity()const { return m_velocity; }
        void SetVelocity(Vector2f velocity) { m_velocity = velocity; }
        float GetMass()const { return m_mass; }
        bool GetIsStatic()const { return mIsStatic; }

        static ComponentType m_type;
    private:
        //Aggregates forces acting on rigidbody
        void CalculateForcesAndMoments(double deltaTime);

        bool mIsStatic = false;
        bool mIsModified = false;

        float m_mass;
        float m_inertia;
        float m_inertiaInverse;

        Vector2f m_velocity;             //velocity in world space
        Vector2f m_localVelocity;        //velocity in model space 
        float m_localAngularVelocity; //angular velocity in model space

        float m_speed;

        Vector2f m_gravity; // Simulation gravity, default value is (0, mass * gravity acceleration)

        Vector2f m_forces;
        float m_moments;
    };

}