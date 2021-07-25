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
        friend class XenonPhysics::Physics2D;
        struct FixedUpdateData 
        {
            Vector2f m_velocity;             //velocity in world space
            Vector2f m_localVelocity;        //velocity in model space 
            float m_localAngularVelocity = 0.0f; //angular velocity in model space
            float m_speed = 0.0f;
            Vector2f m_addedForces;
            Vector2f m_sumOfForces;
            float m_addedMoments =0.0f;
            float m_sumOfMoments =0.0f;

            Vector2f m_deltaDistance;
            float m_deltaRotation =0.0f;
        };

        Rigidbody2D(GameObject* gameobject, bool isStatic, float mass, float inertia);
        virtual ~Rigidbody2D() override;
        virtual ComponentType GetComponentType() const override { return m_type; };

        const FixedUpdateData& PreFixedUpdate(float deltaTime);
        bool FixedUpdate(float deltaTime);     //One time step
        bool FallbackUpdate();
        bool AddForce(const XenonPhysics::Force2D& force);
        bool AddLocalForce(const XenonPhysics::Force2D& force);

        Vector2f GetVelocity()const { return m_currentData.m_velocity; }
        Vector2f GetLocalVelocity()const { return m_currentData.m_localVelocity; }
        float GetAngularVelocity()const { return m_currentData.m_localAngularVelocity; }
        void SetVelocity(Vector2f velocity) { m_currentData.m_velocity = velocity; }
        void SetMass(float mass) { m_mass = mass; }
        float GetMass()const { return m_mass; }
        const Vector2f& GetForce()const { return m_addedForces; }
        //void SetCollision() { m_isCollision = true; }
        bool IsStatic()const { return mIsStatic; }

        static ComponentType m_type;
    private:
        FixedUpdateData& GetCurrentUpdataData() { return m_currentData; }
        FixedUpdateData& GetLastUpdataData() { return m_lastFrameFixedUpdateData; }
        bool AddForce(const Vector2f& force);
        //Aggregates forces acting on rigidbody
        FixedUpdateData& CalculateForcesAndMoments(FixedUpdateData& currentData, double deltaTime);
        void CalculateRigidbodyByUpdateData( FixedUpdateData& currentData, double deltaTime)const;
        void AffectGameobjectInWorld(const FixedUpdateData& data);
        bool mIsStatic = false;
        bool m_isEnableAirDrag = false;
        bool mIsModified = false;
        bool m_isSimulateGravity = true;
        //bool m_isCollision = false;
        //Vector2f m_gravity; // Simulation gravity, default value is (0, mass * gravity acceleration)

        float m_mass;
        float m_inertia;
        float m_inertiaInverse;

        Vector2f m_addedForces;
        float m_addedMoments = 0.0f;

        bool m_isAllowPullBack = false;
        FixedUpdateData m_currentData;
        FixedUpdateData m_lastFrameFixedUpdateData;
    };

}