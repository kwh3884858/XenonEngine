//  Physics2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector2f.h"
namespace XenonPhysics
{
    class Rigidbody2D;

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
            Vector2f mCollisionNormalVec;
            Vector2f mRelativeVelocityVec;
        };

        Physics2D();
        ~Physics2D();

        bool FixedUpdate();

        bool AddRigidbody2D(const Rigidbody2D*const rigidbody);
        bool RemoveRigidbody2D(const Rigidbody2D* const rigidbody);

    private:
        static const float TIMESTEP = 0.1f;
        static const float MINDELTATIME = 0.02f;        //Min delta time
        static const float EPSILON = 0.01f;
        static const float CollisionTolerance = 0.1f;        //collision tolerance
        static const float CoefficientOfRestitution = 0.8f;

        CollisionInfo CheckForCollision(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionInfo CheckForCollisionCircleAndCircle(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionInfo CheckForCollisionCircleAndBox(Rigidbody2D* ball, Rigidbody2D* plane);

        void ApplyImpulse(Rigidbody2D* body1, Rigidbody2D* body2, CollisionInfo info);
        void ApplyImpulseStaic(Rigidbody2D* staticBody, Rigidbody2D* dynamicBody, CollisionInfo info);

        Vector<Rigidbody2D*> mDynamicRigidbodys;
        Vector<Rigidbody2D*> mStaticRigidbodys;
    };

}