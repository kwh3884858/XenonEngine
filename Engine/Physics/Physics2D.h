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
    class Collider2D;
    class CircleCollider2D;
    class BoxCollider2D;
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

            //static
            union {
                Rigidbody2D* m_collisionBody1 = nullptr;
                Collider2D* m_collider1; = nullptr;
            };
            Rigidbody2D* m_collisionBody2 = nullptr; //dynamic
        };

        Physics2D();
        ~Physics2D();

        bool FixedUpdate();

        bool AddRigidbody2D(Rigidbody2D*const rigidbody);
        bool AddCollider2D(Collider2D*const collider);
        bool RemoveRigidbody2D(Rigidbody2D* const rigidbody);

    private:
        static const float TIMESTEP;
        static const float MINDELTATIME;        //Min delta time
        static const float EPSILON;
        static const float CollisionTolerance;        //collision tolerance
        static const float CoefficientOfRestitution;

        CollisionInfo CheckForCollision(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionInfo CheckForCollisionCircleAndCircle(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionType CheckForCollisionCircleAndCircle(CircleCollider2D* body1Collider, CircleCollider2D* body2Collider, Vector2f v1, Vector2f v2);
        CollisionInfo CheckForCollisionCircleAndBox(Rigidbody2D* ball, Rigidbody2D* plane);
        CollisionInfo CheckForCollisionCircleAndBox(CircleCollider2D* ballCollider, BoxCollider2D* boxCollider, Vector2f ballVelocity, Vector2f boxVelocity);

        void ApplyImpulse(CollisionInfo info);
        void ApplyImpulseCollider(CollisionInfo info);
        void ApplyImpulseStaic(CollisionInfo info);

        Vector<Rigidbody2D*> mDynamicRigidbodys;
        Vector<Rigidbody2D*> mStaticRigidbodys;
        Vector<Collider2D*> m_colliders;
    };

}