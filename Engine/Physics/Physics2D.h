//  Physics2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2f.h"
#include "Algorithms/Vector.h"

namespace XenonEngine
{
    class Rigidbody2D;
}
namespace XenonPhysics
{
    using MathLab::Vector2f;
    using Algorithm::Vector;
    using XenonEngine::Rigidbody2D;

    class Physics2D
    {
    public:
        enum CollisionType
        {
            NoCollision,
            IsCollision,
            Penetrating
        };

        struct CollisionInfo 
        {
            CollisionType m_collisionType = CollisionType::NoCollision;
            Vector2f m_collisionNormalVec;
            Vector2f m_relativeVelocityVec;
            Rigidbody2D* m_collisionBody1 = nullptr; //static
            Rigidbody2D* m_collisionBody2 = nullptr; //dynamic
        };

        Physics2D();
        ~Physics2D();

        bool FixedUpdate();

        bool AddRigidbody2D(const Rigidbody2D*const rigidbody);
        bool RemoveRigidbody2D(const Rigidbody2D* const rigidbody);

    private:
         const float TIMESTEP = 0.1f;
         const float MINDELTATIME = 0.02f;        //Min delta time
         const float EPSILON = 0.01f;
         const float CollisionTolerance = 0.1f;        //collision tolerance
         const float CoefficientOfRestitution = 0.8f;

        CollisionInfo CheckForCollision(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionInfo CheckForCollisionCircleAndCircle(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionInfo CheckForCollisionCircleAndBox(Rigidbody2D* ball, Rigidbody2D* plane);

        void ApplyImpulse(CollisionInfo info);
        void ApplyImpulseStaic(CollisionInfo info);

        Vector<Rigidbody2D*> mDynamicRigidbodys;
        Vector<Rigidbody2D*> mStaticRigidbodys;
    };

}