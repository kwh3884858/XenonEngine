#include "Physics2D.h"

#include "Engine\GameObject.h"
#include "Engine\Component\BoxCollider2D.h"
#include "Engine\Component\Transform2D.h"
#include "Engine\Component\CircleCollider2D.h"
#include "Engine\Component\Rigidbody2D.h"

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
                        assert(true == false)
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
        }
        else if (collider != nullptr)
        {
            AddCollider2D(collider);
        }
        else
        {
            assert(true == false);
        }
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
            info.m_collisionType = CheckForCollisionCircleAndCircle(body1, body2);
        }

        else if (type1 == ColliderType::Circle &&type2 == ColliderType::Box)
        {
            info.m_collisionType = CheckForCollisionCircleAndBox(body1, body2);
        }

        else if (type1 == ColliderType::Box && type2 == ColliderType::Circle)
        {
            info.m_collisionType = CheckForCollisionCircleAndBox(body2, body1);
            // TODO
            //if (collisionType.m_collisionType == CollisionType::IsCollision && mCollisionBody1 != nullptr && mCollisionBody2 != nullptr)
            //{
                /*		Rigidbody2D tempRigidbody;
                        memcpy(&tempRigidbody, mCollisionBody1, sizeof(Rigidbody2D));
                        memcpy(mCollisionBody1, mCollisionBody2, sizeof(Rigidbody2D));
                        memcpy(mCollisionBody2, &tempRigidbody, sizeof(Rigidbody2D));*/
                        //}
        }
        else
        {
            assert(true == false);
        }
        return info;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndCircle(Rigidbody2D* body1, Rigidbody2D* body2)
    {
        Collider2D* collider1 = body1->GetGameObject()->GetComponent<Collider2D>();
        Collider2D* collider2 = body2->GetGameObject()->GetComponent<Collider2D>();

        assert(collider1->GetColliderType() == ColliderType::Circle);
        assert(collider2->GetColliderType() == ColliderType::Circle);

        CircleCollider2D* body1Collider = static_cast<CircleCollider2D*>(collider1);
        CircleCollider2D* body2Collider = static_cast<CircleCollider2D*>(collider2);

        CollisionInfo info = CheckForCollisionCircleAndCircleByCollider(body1Collider, body2Collider);
        if (info.m_collisionType == CollisionType::IsCollision)
        {
            info.m_rigidbody1 = body1;
            info.m_rigidbody2 = body2;
        }

        return info;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndBox(Rigidbody2D* ball, Rigidbody2D* box)
    {
        CircleCollider2D* ballCollider = ball->GetGameObject()->GetComponent<CircleCollider2D>();
        BoxCollider2D* boxCollider = box->GetGameObject()->GetComponent<BoxCollider2D>();

        Vector2f ballVelocity = ball->GetVelocity();
        Vector2f boxVelocity = box->GetVelocity();

        CollisionInfo info = CheckForCollisionCircleAndBoxByCollider(ballCollider, boxCollider, ballVelocity, boxVelocity);

        if (info.m_collisionType == CollisionType::IsCollision)
        {
            info.m_rigidbody1 = ball;
            info.m_rigidbody2 = box;
        }

        return info;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionBoxAndBox(Rigidbody2D* box1, Rigidbody2D* box1)
    {
        Collider2D* collider1 = body1->GetGameObject()->GetComponent<Collider2D>();
        Collider2D* collider2 = body2->GetGameObject()->GetComponent<Collider2D>();
        assert(collider1->GetColliderType() == ColliderType::Box);
        assert(collider2->GetColliderType() == ColliderType::Box);
        BoxCollider2D* body1Collider = static_cast<BoxCollider2D*>(collider1);
        BoxCollider2D* body2Collider = static_cast<BoxCollider2D*>(collider2);

        CollisionInfo info = CheckForCollisionBoxAndBoxByCollider(body1Collider, body2Collider);
        if (info.m_collisionType == CollisionType::IsCollision)
        {
            info.m_rigidbody1 = body1;
            info.m_rigidbody2 = body2;
        }

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
            CircleCollider2D* fixedCollider = static_cast<CircleCollider2D*>( collider);
            CircleCollider2D* dynamicCollider = static_cast<CircleCollider2D*>( rigidBodyCollider);
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
            info = CheckForCollisionCircleAndBoxByCollider(dynamicCollider, fixedCollider);
        }
        else if (type1 == Collider2D::Box && type2 == Collider2D::Box)
        {
            BoxCollider2D* fixedCollider = static_cast<BoxCollider2D*>(collider);
            BoxCollider2D* dynamicCollider = static_cast<BoxCollider2D*>(rigidBodyCollider);
            info = CheckForCollisionBoxAndBoxByCollider(fixedCollider, dynamicCollider,Vector2f::Zero, velocity);
        }
        else
        {
            assert(true == false);
        }

        if (info.m_collisionType == CollisionType::IsCollision)
        {
            info.m_collider1 = collider;
            info.m_rigidbody2 = rigidBody;
        }

        return info;

    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndCircleByCollider(CircleCollider2D* body1Collider, CircleCollider2D* body2Collider, Vector2f circleVelocity1, Vector2f circleVelocity2)
    {
        CollisionInfo refInfo;

        float sumOfRaidus = body1Collider->GetRadius() + body2Collider->GetRadius();
        Vector2f relativePositionVector =
            body1Collider->GetGameObject()->GetComponent<Transform2D>()->GetPosition() -
            body2Collider->GetGameObject()->GetComponent<Transform2D>()->GetPosition();

        float s = relativePositionVector.Magnitude() - sumOfRaidus;

        refInfo.m_collisionNormalVec = relativePositionVector.Normalize();

        Vector2f circleVelocity1 = circleVelocity1;
        Vector2f circleVelocity2 = circleVelocity2;
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

    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndBoxByCollider(CircleCollider2D* ballCollider, BoxCollider2D* boxCollider, Vector2f ballVelocity, Vector2f boxVelocity)
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

    Physics2D::CollisionInfo Physics2D::CheckForCollisionBoxAndBoxByCollider(BoxCollider2D* boxCollider1, BoxCollider2D* boxCollider2, Vector2f boxVelocity1, Vector2f boxVelocity2)
    {
        CollisionInfo info;
        info.m_collisionType = CollisionType::NoCollision;

        float sumOfRaidus = boxCollider1->GetRadius() + boxCollider2->GetRadius();
        Vector2f relativePositionVector =
            body1Collider->GetGameObject()->GetComponent<Transform2D>()->GetPosition() -
            body2Collider->GetGameObject()->GetComponent<Transform2D>()->GetPosition();

        float s = relativePositionVector.Magnitude() - sumOfRaidus;
        if (s > CollisionTolerance)
        {
            return info;
        }



        info.m_collisionNormalVec = relativePositionVector.Normalize();

        Vector2f circleVelocity1 = body1->GetVelocity();
        Vector2f circleVelocity2 = body2->GetVelocity();
        info.m_relativeVelocityVec = circleVelocity1 - circleVelocity2;

        float rvn = info.m_collisionNormalVec.Dot(info.m_relativeVelocityVec);

        // They are approaching each other
        if (s <= CollisionTolerance && rvn < 0.0f)
        {
            info.m_collisionType = CollisionType::IsCollision;
        }
        else if (s < -CollisionTolerance)
        {
            info.m_collisionType = CollisionType::Penetrating;
        }
        else
        {
            info.m_collisionType = CollisionType::NoCollision;
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
        Collider2D* collider = info.m_collider1;
        Rigidbody2D* dynamicBody = info.m_rigidbody2;
        assert(collider != nullptr && dynamicBody != nullptr);
        if (collider->IsTrigger() == true)
        {
            collider->GetGameObject()->OnTrigger(dynamicBody->GetGameObject());
        }
        else
        {
            float impulse = info.m_relativeVelocityVec.Dot(info.m_collisionNormalVec) * (1 + CoefficientOfRestitution) * dynamicBody->GetMass();
            Vector2f force = info.m_collisionNormalVec;
            force *= impulse / Physics2D::TIMESTEP;
            dynamicBody->AddForce(force);
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