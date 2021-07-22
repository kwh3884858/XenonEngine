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
    class GameObject;
    class Rigidbody2D;
    class Collider2D;
    class CircleCollider2D;
    class BoxCollider2D;
}
namespace XenonPhysics
{
    using MathLab::Vector2f;
    using Algorithm::Vector;
    using XenonEngine::GameObject;
    using XenonEngine::Rigidbody2D;
    using XenonEngine::Collider2D;
    using XenonEngine::CircleCollider2D;
    using XenonEngine::BoxCollider2D;

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

            // Static Object
            Rigidbody2D* m_rigidbody1 = nullptr;
            Collider2D* m_collider1 = nullptr;
            Rigidbody2D* m_rigidbody2 = nullptr; //dynamic
            Collider2D* m_collider2 = nullptr;
        };

        Physics2D();
        ~Physics2D();

        bool FixedUpdate();
        bool AddGameObject(GameObject* const gameobject);

    private:
        static const float TIMESTEP;
        static const float MINDELTATIME;        //Min delta time
        static const float EPSILON;
        static const float CollisionTolerance;        //collision tolerance
        static const float CoefficientOfRestitution;

        bool AddRigidbody2D(Rigidbody2D*const rigidbody);
        bool AddCollider2D(Collider2D*const collider);
        bool RemoveRigidbody2D(Rigidbody2D* const rigidbody);

        CollisionInfo CheckForCollision(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionInfo CheckForCollisionCircleAndCircle(Rigidbody2D* body1, Rigidbody2D* body2);
        CollisionInfo CheckForCollisionCircleAndBox(Rigidbody2D* ball, Rigidbody2D* plane);
        CollisionInfo CheckForCollisionBoxAndBox(Rigidbody2D* box1, Rigidbody2D* box2);

        CollisionInfo CheckForCollisionColliderAndRigidbody(Collider2D* collider, Rigidbody2D* rigidBody);
        CollisionInfo CheckForCollisionCircleAndCircleByCollider(CircleCollider2D* body1Collider, CircleCollider2D* body2Collider, Vector2f circleVelocity1, Vector2f circleVelocity2);
        CollisionInfo CheckForCollisionCircleAndBoxByCollider(CircleCollider2D* ballCollider, BoxCollider2D* boxCollider, Vector2f ballVelocity, Vector2f boxVelocity);
        CollisionInfo CheckForCollisionBoxAndBoxByCollider(BoxCollider2D* boxCollider1, BoxCollider2D* boxCollider2, Vector2f boxVelocity1, Vector2f boxVelocity2);

        void ApplyImpulse(CollisionInfo info);
        void ApplyImpulseCollider(CollisionInfo info);
        void ApplyImpulseStaic(CollisionInfo info);

        Vector<Rigidbody2D*> m_dynamicRigidbodys;
        Vector<Rigidbody2D*> mStaticRigidbodys;
        Vector<Collider2D*> m_colliders;
    };
}