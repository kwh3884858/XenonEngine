//  Physics2D.cpp
//  XenonEngine
//
//  Created by whkong on 2021-7-20.
//  Copyright (c) 2021 whkong. All rights reserved.
#include "Physics2D.h"

#include "Engine\GameObject.h"
#include "Engine\Component\BoxCollider2D.h"
#include "Engine\Component\Transform2D.h"
#include "Engine\Component\CircleCollider2D.h"
#include "Engine\Component\Rigidbody2D.h"
#include "MathLab\MathLib.h"
#include "Engine\Physics\Force2D.h"

#include <cassert>
namespace XenonPhysics
{
    using XenonEngine::GameObject;
    using XenonEngine::Collider2D;
    using XenonEngine::BoxCollider2D;
    using XenonEngine::Transform2D;
    using XenonEngine::CircleCollider2D;
    using XenonEngine::Rigidbody2D;
    using XenonEngine::ComponentType;
    using MathLab::Vector3f;
    using ColliderType = Collider2D::ColliderType;

    Physics2D::Physics2D()
    {
    }

    Physics2D::~Physics2D()
    {
    }

    bool Physics2D::FixedUpdate()
    {
        float deltaTime = TIMESTEP;

        CollisionInfo collisionInfo;

        int count = 0;

        //Delta time must bigger than 0
        if (deltaTime <= EPSILON)
        {
            return true;
        }

        //Update position
        for (int i = 0; i < m_dynamicRigidbodys.Count(); i++)
        {
            m_dynamicRigidbodys[i]->FixedUpdate(deltaTime);
        }

        //Check collision
        bool tryAgain = true;
        Rigidbody2D tmpBody1(nullptr, true, 0, 0);
        Rigidbody2D tmpBody2(nullptr, true, 0, 0);
        BoxCollider2D tmpBoxCollider(nullptr);
        CircleCollider2D tmpCircleCollider(nullptr);

        // For dynamic and dynamic
        for (int i = 0; i < m_dynamicRigidbodys.Count() - 1; i++) {
            for (int j = i + 1; j < m_dynamicRigidbodys.Count(); j++)
            {
                while (tryAgain && deltaTime > MINDELTATIME)
                {
                    tryAgain = false;
                    memcpy(&tmpBody1, m_dynamicRigidbodys[i], sizeof(Rigidbody2D));
                    memcpy(&tmpBody2, m_dynamicRigidbodys[j], sizeof(Rigidbody2D));

                    collisionInfo.m_rigidbody1 = nullptr;
                    collisionInfo.m_rigidbody2 = nullptr;

                    collisionInfo = CheckForCollision(&tmpBody1, &tmpBody2);

                    if (collisionInfo.m_collisionType == CollisionType::Penetrating)
                    {
                        deltaTime /= 2;
                        tryAgain = true;
                    }
                    else if (collisionInfo.m_collisionType == CollisionType::IsCollision)
                    {
                        if (collisionInfo.m_rigidbody1 != nullptr && collisionInfo.m_rigidbody2 != nullptr)
                        {
                            ApplyImpulse(collisionInfo);
                        }
                    }
                }
                tryAgain = true;

                if (collisionInfo.m_collisionType != CollisionType::Penetrating)
                {
                    memcpy(m_dynamicRigidbodys[i], &tmpBody1, sizeof(Rigidbody2D));
                    memcpy(m_dynamicRigidbodys[j], &tmpBody2, sizeof(Rigidbody2D));
                }
            }
        }

        // For dynamic and static 
        for (int i = 0; i < mStaticRigidbodys.Count(); i++)
        {
            for (int j = 0; j < m_dynamicRigidbodys.Count(); j++)
            {
                while (tryAgain && deltaTime > MINDELTATIME)
                {
                    tryAgain = false;
                    memcpy(&tmpBody1, mStaticRigidbodys[i], sizeof(Rigidbody2D));
                    memcpy(&tmpBody2, m_dynamicRigidbodys[j], sizeof(Rigidbody2D));

                    collisionInfo.m_rigidbody1 = nullptr;
                    collisionInfo.m_rigidbody2 = nullptr;

                    collisionInfo = CheckForCollision(&tmpBody1, &tmpBody2);

                    if (collisionInfo.m_collisionType == CollisionType::Penetrating)
                    {
                        deltaTime /= 2;
                        tryAgain = true;
                    }
                    else if (collisionInfo.m_collisionType == CollisionType::IsCollision)
                    {
                        if (collisionInfo.m_rigidbody1 != nullptr && collisionInfo.m_rigidbody2 != nullptr)
                        {
                            ApplyImpulseStaic(collisionInfo);
                        }
                    }
                }
                tryAgain = true;

                if (collisionInfo.m_collisionType != CollisionType::Penetrating)
                {
                    memcpy(mStaticRigidbodys[i], &tmpBody1, sizeof(Rigidbody2D));
                    memcpy(m_dynamicRigidbodys[j], &tmpBody2, sizeof(Rigidbody2D));
                }
            }
        }

        // For dynamic and collider
        for (int rigidbodyIndex = 0; rigidbodyIndex < m_dynamicRigidbodys.Count(); rigidbodyIndex++)
        {
            for (int colliderIndex = 0; colliderIndex < m_colliders.Count(); colliderIndex++)
            {
                while (tryAgain && deltaTime > MINDELTATIME)
                {
                    tryAgain = false;
                    collisionInfo.m_rigidbody1 = nullptr;
                    collisionInfo.m_rigidbody2 = nullptr;
                    memcpy(&tmpBody2, m_dynamicRigidbodys[rigidbodyIndex], sizeof(Rigidbody2D));

                    ColliderType colliderType = m_colliders[colliderIndex]->GetColliderType();
                    switch (colliderType)
                    {
                    case ColliderType::Box:
                    {
                        memcpy(&tmpBoxCollider, m_colliders[colliderIndex], sizeof(BoxCollider2D));
                        collisionInfo = CheckForCollisionColliderAndRigidbody(&tmpBoxCollider, &tmpBody2);
                    }
                    break;
                    case ColliderType::Circle:
                    {
                        memcpy(&tmpCircleCollider, m_colliders[colliderIndex], sizeof(CircleCollider2D));
                        collisionInfo = CheckForCollisionColliderAndRigidbody(&tmpCircleCollider, &tmpBody2);
                    }
                    break;
                    default:
                        assert(true == false);
                    }

                    if (collisionInfo.m_collisionType == CollisionType::Penetrating)
                    {
                        deltaTime /= 2;
                        tryAgain = true;
                    }
                    else if (collisionInfo.m_collisionType == CollisionType::IsCollision)
                    {
                        if (collisionInfo.m_rigidbody1 != nullptr && collisionInfo.m_rigidbody2 != nullptr)
                        {
                            ApplyImpulseCollider(collisionInfo);
                        }
                    }
                }
                tryAgain = true;

                if (collisionInfo.m_collisionType != CollisionType::Penetrating)
                {
                    ColliderType colliderType = m_colliders[colliderIndex]->GetColliderType();
                    switch (colliderType)
                    {
                    case ColliderType::Box:
                        memcpy(m_colliders[colliderIndex], &tmpBoxCollider, sizeof(BoxCollider2D));
                        break;
                    case ColliderType::Circle:
                        memcpy(m_colliders[colliderIndex], &tmpBoxCollider, sizeof(BoxCollider2D));
                        break;
                    default:
                        assert(true == false);
                    }
                    memcpy(m_dynamicRigidbodys[rigidbodyIndex], &tmpBody2, sizeof(Rigidbody2D));
                }
            }
        }
        return true;
    }

    bool Physics2D::AddGameObject(GameObject* const gameobject)
    {
        Rigidbody2D* rigidbody = gameobject->GetComponent<Rigidbody2D>();
        Collider2D* collider = gameobject->GetComponent<Collider2D>();

        if (rigidbody != nullptr)
        {
            AddRigidbody2D(rigidbody);
            return true;
        }
        else if (collider != nullptr)
        {
            AddCollider2D(collider);
            return true;
        }
        else
        {
            assert(true == false);
        }
        return false;
    }

    bool Physics2D::AddRigidbody2D(Rigidbody2D*const rigidbody)
    {
        if (rigidbody->IsStatic())
        {
            return  mStaticRigidbodys.Add(rigidbody);
        }
        else
        {
            return m_dynamicRigidbodys.Add(rigidbody);
        }
    }

    bool Physics2D::AddCollider2D(Collider2D*const collider)
    {
        Rigidbody2D* rigidbody = collider->GetGameObject()->GetComponent<Rigidbody2D>();
        assert(rigidbody == nullptr);
        if (rigidbody == nullptr)
        {
            m_colliders.Add(collider);
            return true;
        }
        else
        {
            assert(true == false);
            return false;
        }
    }

    bool Physics2D::RemoveRigidbody2D(Rigidbody2D* const rigidbody)
    {
        if (rigidbody->IsStatic())
        {
            return mStaticRigidbodys.Remove(rigidbody);
        }
        else
        {
            return  m_dynamicRigidbodys.Remove(rigidbody);
        }
    }


    Physics2D::CollisionInfo Physics2D::CheckForCollision(Rigidbody2D* body1, Rigidbody2D* body2)
    {
        CollisionInfo info;

        //do type check, and call suitable function
        Collider2D* collider1 = body1->GetGameObject()->GetComponent<Collider2D>();
        Collider2D* collider2 = body2->GetGameObject()->GetComponent<Collider2D>();

        if (collider1 == nullptr || collider2 == nullptr)
        {
            info.m_collisionType = CollisionType::NoCollision;
            return info;
        }

        ColliderType type1 = collider1->GetColliderType();
        ColliderType type2 = collider2->GetColliderType();

        if (type1 == ColliderType::Circle && type2 == ColliderType::Circle)
        {
            info = CheckForCollisionCircleAndCircle(body1, body2);
        }
        else if (type1 == ColliderType::Circle &&type2 == ColliderType::Box)
        {
            info = CheckForCollisionCircleAndBox(body1, body2);
        }
        else if (type1 == ColliderType::Box && type2 == ColliderType::Circle)
        {
            info = CheckForCollisionCircleAndBox(body2, body1);
        }
        else if (type1 == ColliderType::Box && type2 == ColliderType::Box)
        {
            info = CheckForCollisionBoxAndBox(body1, body2);
        }
        else
        {
            assert(true == false);
        }

        info.m_rigidbody1 = body1;
        info.m_collider1 = collider1;
        info.m_rigidbody1 = body1;
        info.m_collider2 = collider2;

        return info;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndCircle(Rigidbody2D* body1, Rigidbody2D* body2)
    {
        CircleCollider2D* body1Collider = body1->GetGameObject()->GetComponent<CircleCollider2D>();
        CircleCollider2D* body2Collider = body2->GetGameObject()->GetComponent<CircleCollider2D>();
        assert(body1Collider != nullptr);
        assert(body2Collider != nullptr);
        assert(body1Collider->GetColliderType() == ColliderType::Circle);
        assert(body2Collider->GetColliderType() == ColliderType::Circle);

        CollisionInfo info = CheckForCollisionCircleAndCircleByCollider(body1Collider, body2Collider, body1->GetVelocity(), body2->GetVelocity());
        return info;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndBox(Rigidbody2D* ball, Rigidbody2D* box)
    {
        CircleCollider2D* ballCollider = ball->GetGameObject()->GetComponent<CircleCollider2D>();
        BoxCollider2D* boxCollider = box->GetGameObject()->GetComponent<BoxCollider2D>();
        assert(ballCollider != nullptr);
        assert(boxCollider != nullptr);
        assert(ballCollider->GetColliderType() == ColliderType::Circle);
        assert(boxCollider->GetColliderType() == ColliderType::Box);

        CollisionInfo info = CheckForCollisionCircleAndBoxByCollider(ballCollider, boxCollider, ball->GetVelocity(), box->GetVelocity());
        return info;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionBoxAndBox(Rigidbody2D* box1, Rigidbody2D* box2)
    {
        BoxCollider2D* body1Collider = box1->GetGameObject()->GetComponent<BoxCollider2D>();
        BoxCollider2D* body2Collider = box2->GetGameObject()->GetComponent<BoxCollider2D>();
        assert(body1Collider != nullptr);
        assert(body2Collider != nullptr);
        assert(body1Collider->GetColliderType() == ColliderType::Box);
        assert(body2Collider->GetColliderType() == ColliderType::Box);

        CollisionInfo info = CheckForCollisionBoxAndBoxByCollider(body1Collider, body2Collider, box1->GetVelocity(), box2->GetVelocity());
        return info;
    }


    Physics2D::CollisionInfo Physics2D::CheckForCollisionColliderAndRigidbody(Collider2D* collider, Rigidbody2D* rigidBody)
    {
        CollisionInfo info;

        //do type check, and call suitable function
        Collider2D* rigidBodyCollider = rigidBody->GetGameObject()->GetComponent<Collider2D>();

        if (collider == nullptr || rigidBodyCollider == nullptr)
        {
            info.m_collisionType = CollisionType::NoCollision;
            return info;
        }

        ColliderType type1 = collider->GetColliderType();
        ColliderType type2 = rigidBodyCollider->GetColliderType();
        Vector2f velocity = rigidBody->GetVelocity();

        if (type1 == ColliderType::Circle && type2 == ColliderType::Circle)
        {
            CircleCollider2D* fixedCollider = static_cast<CircleCollider2D*>(collider);
            CircleCollider2D* dynamicCollider = static_cast<CircleCollider2D*>(rigidBodyCollider);
            info = CheckForCollisionCircleAndCircleByCollider(fixedCollider, dynamicCollider, Vector2f::Zero, velocity);
        }
        else if (type1 == ColliderType::Circle &&type2 == ColliderType::Box)
        {
            CircleCollider2D* fixedCollider = static_cast<CircleCollider2D*>(collider);
            BoxCollider2D* dynamicCollider = static_cast<BoxCollider2D*>(rigidBodyCollider);
            info = CheckForCollisionCircleAndBoxByCollider(fixedCollider, dynamicCollider, Vector2f::Zero, velocity);
        }
        else if (type1 == ColliderType::Box && type2 == ColliderType::Circle)
        {
            BoxCollider2D* fixedCollider = static_cast<BoxCollider2D*>(collider);
            CircleCollider2D* dynamicCollider = static_cast<CircleCollider2D*>(rigidBodyCollider);
            info = CheckForCollisionCircleAndBoxByCollider(dynamicCollider, fixedCollider, Vector2f::Zero, velocity);
        }
        else if (type1 == Collider2D::Box && type2 == Collider2D::Box)
        {
            BoxCollider2D* fixedCollider = static_cast<BoxCollider2D*>(collider);
            BoxCollider2D* dynamicCollider = static_cast<BoxCollider2D*>(rigidBodyCollider);
            info = CheckForCollisionBoxAndBoxByCollider(fixedCollider, dynamicCollider, Vector2f::Zero, velocity);
        }
        else
        {
            assert(true == false);
        }

        info.m_collider1 = collider;
        info.m_rigidbody2 = rigidBody;
        info.m_collider2 = rigidBodyCollider;
        return info;

    }

    XenonPhysics::Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndCircleByCollider(CircleCollider2D* body1Collider, CircleCollider2D* body2Collider, Vector2f circleVelocity1, Vector2f circleVelocity2) const
    {
        CollisionInfo refInfo;

        float sumOfRaidus = body1Collider->GetRadius() + body2Collider->GetRadius();
        Vector2f relativePositionVector =
            body1Collider->GetGameObject()->GetComponent<Transform2D>()->GetPosition() -
            body2Collider->GetGameObject()->GetComponent<Transform2D>()->GetPosition();

        float s = relativePositionVector.Magnitude() - sumOfRaidus;

        refInfo.m_collisionNormalVec = relativePositionVector.Normalize();
        refInfo.m_relativeVelocityVec = circleVelocity1 - circleVelocity2;
        float rvn = refInfo.m_collisionNormalVec.Dot(refInfo.m_relativeVelocityVec);

        // They are approaching each other
        if (s <= CollisionTolerance && rvn < 0.0f)
        {
            refInfo.m_collisionType = CollisionType::IsCollision;
        }
        else if (s < -CollisionTolerance)
        {
            refInfo.m_collisionType = CollisionType::Penetrating;
        }
        else
        {
            refInfo.m_collisionType = CollisionType::NoCollision;
        }
        return refInfo;
    }

    XenonPhysics::Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndBoxByCollider(CircleCollider2D* ballCollider, BoxCollider2D* boxCollider, Vector2f ballVelocity, Vector2f boxVelocity) const
    {
        CollisionInfo info;

        //radius
        float radius = ballCollider->GetRadius();

        //Quick test
        Vector2f circlePosition = ballCollider->GetGameObject()->GetComponent<Transform2D>()->GetPosition();
        Vector2f boxPosition = boxCollider->GetGameObject()->GetComponent<Transform2D>()->GetPosition();
        Vector2f boxMinPoint(0, 0);
        Vector2f size = boxCollider->GetSize();

        boxMinPoint -= size / 2;

        /*
        Our code will first test which edge of the rectangle is closest to the circle,
        then see if there is a collision using the Pythagorean Theorem.
        Let�s create a temporary variable for the square�s closest X/Y edges.
        We�ll set them as the circle�s position to start:
        */
        Vector2f closestToCircle = circlePosition;
        if (closestToCircle.x < boxMinPoint.x) closestToCircle.x = boxMinPoint.x;
        if (closestToCircle.y < boxMinPoint.y) closestToCircle.y = boxMinPoint.y;
        if (closestToCircle.x > boxMinPoint.x + size.x) closestToCircle.x = boxMinPoint.x + size.x;
        if (closestToCircle.y > boxMinPoint.y + size.y) closestToCircle.y = boxMinPoint.y + size.y;

        /*
        Now that we know which edges to check,
        we run the Pythagorean Theorem code using the circle�s center and the two edges we found above:
        */
        float distX = circlePosition.x - closestToCircle.x;
        float distY = circlePosition.y - closestToCircle.y;
        float distance = sqrtf(distX * distX + distY * distY);
        float relativeDistance = distance - radius;

        info.m_collisionNormalVec = (circlePosition - closestToCircle).Normalize();
        info.m_relativeVelocityVec = ballVelocity - boxVelocity;
        float rvn = info.m_collisionNormalVec.Dot(info.m_relativeVelocityVec);

        if (relativeDistance < CollisionTolerance && rvn < 0.0f)
        {
            info.m_collisionType = CollisionType::IsCollision;
        }
        else if (relativeDistance < -CollisionTolerance)
        {
            info.m_collisionType = CollisionType::Penetrating;
        }
        else
        {
            info.m_collisionType = CollisionType::NoCollision;
        }

        return info;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionBoxAndBoxByCollider(BoxCollider2D* boxCollider1, BoxCollider2D* boxCollider2, Vector2f boxVelocity1, Vector2f boxVelocity2) const
    {
        CollisionInfo info;
        info.m_collisionType = CollisionType::NoCollision;

        float sumOfRaidus = boxCollider1->GetRadius() + boxCollider2->GetRadius();

        Transform2D* box1Transform = boxCollider1->GetGameObject()->GetComponent<Transform2D>();
        Transform2D* box2Transform = boxCollider2->GetGameObject()->GetComponent<Transform2D>();
        Vector2f relativePositionVector = box1Transform->GetPosition() - box2Transform->GetPosition();
        float s = relativePositionVector.Magnitude() - sumOfRaidus;
        if (s > CollisionTolerance)
        {
            return info;
        }
        info.m_collisionNormalVec = relativePositionVector;

        Vector2f box1Pos = box1Transform->GetPosition();
        Vector2f box2Pos = box2Transform->GetPosition();
        Vector2f box1extent = boxCollider1->GetSize() / 2;
        Vector2f box2extent = boxCollider2->GetSize() / 2;
        // Top left, Top right, Button left, Button right
        Vector2f box1InWorld[4];
        box1InWorld[0].x = box1Pos.x - box1extent.x;
        box1InWorld[0].y = box1Pos.y + box1extent.y;
        box1InWorld[1].x = box1Pos.x + box1extent.x;
        box1InWorld[1].y = box1Pos.y + box1extent.y;
        box1InWorld[2].x = box1Pos.x - box1extent.x;
        box1InWorld[2].y = box1Pos.y - box1extent.y;
        box1InWorld[3].x = box1Pos.x - box1extent.x;
        box1InWorld[3].y = box1Pos.y + box1extent.y;
        Vector2f box2InWorld[4];
        box2InWorld[0].x = box2Pos.x - box2extent.x;
        box2InWorld[0].y = box2Pos.y + box2extent.y;
        box2InWorld[1].x = box2Pos.x + box2extent.x;
        box2InWorld[1].y = box2Pos.y + box2extent.y;
        box2InWorld[2].x = box2Pos.x - box2extent.x;
        box2InWorld[2].y = box2Pos.y - box2extent.y;
        box2InWorld[3].x = box2Pos.x + box2extent.x;
        box2InWorld[3].y = box2Pos.y - box2extent.y;

        Rigidbody2D* boxRigidbody1 = boxCollider1->GetGameObject()->GetComponent<Rigidbody2D>();
        Rigidbody2D* boxRigidbody2 = boxCollider2->GetGameObject()->GetComponent< Rigidbody2D >();
        if (boxRigidbody1)
        {
            info.m_rigidbody1 = boxRigidbody1;
        }
        info.m_collider1 = boxCollider1;
        if (boxRigidbody2)
        {
            info.m_rigidbody1 = boxRigidbody2;
        }
        info.m_collider2 = boxCollider2;

        // Vertex and Vertex
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Vector2f velocity1(0, 0);
                if (boxRigidbody1)
                {
                    Vector2f collisionPoint1 = box1InWorld[i] - box1Transform->GetPosition();
                    Vector2f tangentOfangularVelocity1(-collisionPoint1.y *  boxRigidbody1->GetAngularVelocity(), -collisionPoint1.x * boxRigidbody1->GetAngularVelocity());
                    velocity1 = tangentOfangularVelocity1 + boxRigidbody1->GetLocalVelocity();
                    velocity1 = MathLab::Rotate2D(velocity1, box1Transform->GetOrientation());
                }
                Vector2f velocity2(0, 0);
                if (boxRigidbody2)
                {
                    Vector2f collisionPoint2 = box2InWorld[i] - box2Transform->GetPosition();
                    Vector2f tangentOfangularVelocity2(-collisionPoint2.y * boxRigidbody2->GetAngularVelocity(), -collisionPoint2.x * boxRigidbody2->GetAngularVelocity());
                    velocity2 = tangentOfangularVelocity2 + boxRigidbody2->GetLocalVelocity();
                    velocity2 = MathLab::Rotate2D(velocity2, box2Transform->GetOrientation());
                }
                Vector2f relativeVelocityVector = velocity1 - velocity2;

                if ((box1InWorld[i] - box2InWorld[j]).Magnitude() < Physics2D::CollisionTolerance)
                {
                    info.m_collisionType = CollisionType::IsCollision;
                    info.m_relativeVelocityVec = relativeVelocityVector;
                }
            }
        }

        // Vertex and Edge
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Vector2f edge;
                if (j == 0)
                {
                    edge = box2InWorld[0] - box2InWorld[1];
                }
                else if (j == 1)
                {
                    edge = box2InWorld[1] - box2InWorld[3];
                }
                else if (j == 3)
                {
                    edge = box2InWorld[3] - box2InWorld[2];
                }
                else if (j == 2)
                {
                    edge = box2InWorld[2] - box2InWorld[0];
                }
                Vector2f edgeNormal = edge.Normalize();

                Vector2f edgeVertexToCollisionVertex = box1InWorld[i] - box2InWorld[j];
                Vector2f vertexProjectile = (edgeVertexToCollisionVertex.Dot(edgeNormal)) * edgeNormal;

                float zAxisOfMoment = edgeNormal.Cross(edgeVertexToCollisionVertex);
                Vector2f collisionNormal(-zAxisOfMoment * edgeNormal.y, zAxisOfMoment * edgeNormal.x);
                collisionNormal = collisionNormal.Normalize();

                Vector2f velocity1(0, 0);
                if (boxRigidbody1)
                {
                    Vector2f collisionPoint1 = box1InWorld[i] - box1Transform->GetPosition();
                    Vector2f tangentOfangularVelocity1(-collisionPoint1.y *  boxRigidbody1->GetAngularVelocity(), -collisionPoint1.x * boxRigidbody1->GetAngularVelocity());
                    velocity1 = tangentOfangularVelocity1 + boxRigidbody1->GetLocalVelocity();
                    velocity1 = MathLab::Rotate2D(velocity1, box1Transform->GetOrientation());
                }
                Vector2f velocity2(0, 0);
                if (boxRigidbody2)
                {
                    Vector2f collisionPoint2 = box2InWorld[i] - box2Transform->GetPosition();
                    Vector2f tangentOfangularVelocity2(-collisionPoint2.y * boxRigidbody2->GetAngularVelocity(), -collisionPoint2.x * boxRigidbody2->GetAngularVelocity());
                    velocity2 = tangentOfangularVelocity2 + boxRigidbody2->GetLocalVelocity();
                    velocity2 = MathLab::Rotate2D(velocity2, box2Transform->GetOrientation());
                }
                Vector2f relativeVelocityVector = velocity1 - velocity2;

                if (vertexProjectile.Magnitude() > 0.0f &&
                    vertexProjectile.Magnitude() < edge.Magnitude() &&
                    zAxisOfMoment < Physics2D::CollisionTolerance &&
                    relativeVelocityVector.Dot(collisionNormal) < 0)
                {
                    info.m_collisionType = CollisionType::IsCollision;
                    info.m_collisionNormalVec = collisionNormal;
                    info.m_relativeVelocityVec = relativeVelocityVector;
                }
            }
        }

        // inter penetrate
        for (int i = 0; i < 4; i++)
        {
            bool isPentrate = true;
            for (int j = 0; j < 4; j++)
            {
                Vector2f edge;
                if (j == 0)
                {
                    edge = box2InWorld[0] - box2InWorld[1];
                }
                else if (j == 1)
                {
                    edge = box2InWorld[1] - box2InWorld[3];
                }
                else if (j == 3)
                {
                    edge = box2InWorld[3] - box2InWorld[2];
                }
                else if (j == 2)
                {
                    edge = box2InWorld[2] - box2InWorld[0];
                }
                Vector2f edgeToVertex = box2InWorld[j] - box1InWorld[i];
                if (edge.Dot(edgeToVertex) < 0)
                {
                    isPentrate = false;
                }
            }
            if (isPentrate)
            {
                info.m_collisionType = CollisionType::Penetrating;
            }
        }
        return info;
    }


    void Physics2D::ApplyImpulse(CollisionInfo info)
    {
        Rigidbody2D* body1 = info.m_rigidbody1;
        Rigidbody2D* body2 = info.m_rigidbody2;

        float j = (info.m_relativeVelocityVec.Dot(info.m_collisionNormalVec) *
            -(1 + CoefficientOfRestitution)) /
            (info.m_collisionNormalVec.Dot(info.m_collisionNormalVec) *
            (1 / body1->GetMass() + 1 / body2->GetMass()));

        Vector2f body1Velocity = body1->GetVelocity();
        Vector2f body2Velocity = body2->GetVelocity();

        body1Velocity += (j * info.m_collisionNormalVec) / body1->GetMass();
        body2Velocity -= (j * info.m_collisionNormalVec) / body2->GetMass();

        body1->SetVelocity(body1Velocity);
        body2->SetVelocity(body2Velocity);
        //TODO More accurate impulse
    }

    void Physics2D::ApplyImpulseCollider(CollisionInfo info)
    {
        Collider2D* collider = nullptr;
        Rigidbody2D* dynamicBody = nullptr;
        if (info.m_rigidbody1 != nullptr && info.m_collider2 != nullptr)
        {
            dynamicBody = info.m_rigidbody1;
            collider = info.m_collider2;
        }
        else if (info.m_collider1 != nullptr && info.m_rigidbody2 != nullptr)
        {
            collider = info.m_collider1;
            dynamicBody = info.m_rigidbody2;
        }
        assert(collider != nullptr && dynamicBody != nullptr);
        if (collider->IsTrigger() == true)
        {
            collider->GetGameObject()->OnTrigger(dynamicBody->GetGameObject());
        }
        else
        {
            float impulse = info.m_relativeVelocityVec.Dot(info.m_collisionNormalVec) * (1 + CoefficientOfRestitution) * dynamicBody->GetMass();
            XenonPhysics::Force2D force2D;
            force2D.m_forceDirection = info.m_collisionNormalVec;
            force2D.fvalue = impulse / Physics2D::TIMESTEP;
            dynamicBody->AddForce(force2D);
        }
    }

    void Physics2D::ApplyImpulseStaic(CollisionInfo info)
    {
        Vector2f dynamicBodyVelocity;
        Rigidbody2D* staticBody = info.m_rigidbody1;
        Rigidbody2D* dynamicBody = info.m_rigidbody2;
        if (staticBody->IsStatic() == true)
        {
            if (staticBody->GetGameObject()->GetComponent<Collider2D>()->IsTrigger() == true)
            {
                staticBody->GetGameObject()->OnTrigger(dynamicBody->GetGameObject());
            }
            else
            {
                dynamicBodyVelocity = dynamicBody->GetVelocity();
                dynamicBodyVelocity += -2 * info.m_relativeVelocityVec.Dot(info.m_collisionNormalVec) * info.m_collisionNormalVec;
                dynamicBody->SetVelocity(dynamicBodyVelocity);
            }
        }
        else
        {
            if (dynamicBody->GetGameObject()->GetComponent<Collider2D>()->IsTrigger() == true)
            {
                dynamicBody->GetGameObject()->OnTrigger(staticBody->GetGameObject());
            }
            else
            {
                dynamicBodyVelocity = staticBody->GetVelocity();
                dynamicBodyVelocity += -2 * info.m_relativeVelocityVec.Dot(info.m_collisionNormalVec) * info.m_collisionNormalVec;
                staticBody->SetVelocity(dynamicBodyVelocity);
            }
        }

        return;
        //TODO
        //Vector3f dynamicBodyVelocity = body2->getVelocityVec();
        /*j = (mRelativeVelocityVec.Dot(mCollisionNormalVec) *
            -2) /
            (mCollisionNormalVec.Dot(mCollisionNormalVec) *
                (1 / staticBody->getMass() + 1 / dynamicBody->getMass()));*/

                //float DotResult = ;
                //dynamicBodyVelocity += -2 * mRelativeVelocityVec.Dot(mCollisionNormalVec) * mCollisionNormalVec;
                /*Vector3f temp1 = dynamicBodyVelocity + temp;
                dynamicBodyVelocity = temp1;*/
                //Vector3f body1Velocity = staticBody->getVelocityVec();
                //XMVECTOR body2Velocity = body2->getVelocity();
                //body1Velocity += (j * mCollisionNormal) / body1->getMass();
                //dynamicBodyVelocity -= (j * mCollisionNormalVec) / dynamicBody->getMass();

                //body1->setVelocity(body1Velocity);
                //body2->setVelocity(dynamicBodyVelocity);
    }

    const float Physics2D::TIMESTEP = 0.1f;
    const float Physics2D::MINDELTATIME = 0.02f;
    const float Physics2D::EPSILON = 0.01f;
    const float Physics2D::CollisionTolerance = 0.1f;
    const float Physics2D::CoefficientOfRestitution = 0.8f;

}