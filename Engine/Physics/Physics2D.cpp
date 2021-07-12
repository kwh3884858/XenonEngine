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

        bool penetrated = false;
        int count = 0;

        //Delta time must bigger than 0
        if (deltaTime <= EPSILON)
        {
            return true;
        }

        //Update position
        for (int i = 0; i < mDynamicRigidbodys.Count(); i++)
        {
            mDynamicRigidbodys[i]->FixedUpdate(deltaTime);
        }

        //Check collision
        bool tryAgain = true;
        Rigidbody2D body1(nullptr, true, 0, 0);
        Rigidbody2D body2(nullptr, true, 0, 0);

        for (int i = 0; i < mDynamicRigidbodys.Count() - 1; i++) {
            for (int j = i + 1; j < mDynamicRigidbodys.Count(); j++)
            {
                while (tryAgain && deltaTime > MINDELTATIME)
                {
                    tryAgain = false;
                    memcpy(&body1, mDynamicRigidbodys[i], sizeof(Rigidbody2D));
                    memcpy(&body2, mDynamicRigidbodys[j], sizeof(Rigidbody2D));

                    collisionInfo.m_collisionBody1 = nullptr;
                    collisionInfo.m_collisionBody2 = nullptr;

                    collisionInfo = CheckForCollision(&body1, &body2);

                    if (collisionInfo.m_collisionType == CollisionType::Penetrating)
                    {
                        deltaTime /= 2;
                        tryAgain = true;
                        penetrated = true;
                    }
                    else if (collisionInfo.m_collisionType == CollisionType::IsCollision)
                    {
                        if (collisionInfo.m_collisionBody1 != nullptr && collisionInfo.m_collisionBody2 != nullptr)
                        {
                            ApplyImpulse(collisionInfo);
                        }
                    }
                }
                tryAgain = true;

                if (!penetrated)
                {
                    memcpy(mDynamicRigidbodys[i], &body1, sizeof(Rigidbody2D));
                    memcpy(mDynamicRigidbodys[j], &body2, sizeof(Rigidbody2D));
                }
            }
        }

        //for static and dynamic
        for (int i = 0; i < mStaticRigidbodys.Count(); i++)
        {
            for (int j = 0; j < mStaticRigidbodys.Count(); j++)
            {
                while (tryAgain && deltaTime > MINDELTATIME)
                {
                    tryAgain = false;
                    memcpy(&body1, mStaticRigidbodys[i], sizeof(Rigidbody2D));
                    memcpy(&body2, mDynamicRigidbodys[j], sizeof(Rigidbody2D));

                    //body1.fixedUpdate(deltaTime);
                    //body2.fixedUpdate(deltaTime);

                    collisionInfo.m_collisionBody1 = nullptr;
                    collisionInfo.m_collisionBody2 = nullptr;

                    collisionInfo = CheckForCollision(&body1, &body2);

                    if (collisionInfo.m_collisionType == CollisionType::Penetrating)
                    {
                        deltaTime /= 2;
                        tryAgain = true;
                        penetrated = true;
                    }
                    else if (collisionInfo.m_collisionType == CollisionType::IsCollision)
                    {
                        if (collisionInfo.m_collisionBody1 != nullptr && collisionInfo.m_collisionBody2 != nullptr)
                        {
                            ApplyImpulseStaic(collisionInfo);
                        }
                    }
                }
                tryAgain = true;

                if (!penetrated)
                {
                    memcpy(mStaticRigidbodys[i], &body1, sizeof(Rigidbody2D));
                    memcpy(mDynamicRigidbodys[j], &body2, sizeof(Rigidbody2D));
                }
            }

        }
        return true;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollision(Rigidbody2D* body1, Rigidbody2D* body2)
    {
        CollisionInfo collisionType;

        //do type check, and call suitable function
        Collider2D* collider1 = body1->GetGameObject()->GetComponent<Collider2D>();
        Collider2D* collider2 = body2->GetGameObject()->GetComponent<Collider2D>();
        ColliderType type1 = collider1->GetColliderType();
        ColliderType type2 = collider2->GetColliderType();

        if (type1 == ColliderType::Circle && type2 == ColliderType::Circle)
        {
            collisionType = CheckForCollisionCircleAndCircle(body1, body2);
        }

        else if (type1 == ColliderType::Circle &&type2 == ColliderType::Box)
        {
            collisionType = CheckForCollisionCircleAndBox(body1, body2);
        }

        else if (type1 == ColliderType::Box && type2 == ColliderType::Circle)
        {
            collisionType = CheckForCollisionCircleAndBox(body2, body1);
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
        return collisionType;
    }


    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndCircle(Rigidbody2D* body1, Rigidbody2D* body2)
    {
        CollisionInfo retval;

        Collider2D* collider1 = body1->GetGameObject()->GetComponent<Collider2D>();
        Collider2D* collider2 = body2->GetGameObject()->GetComponent<Collider2D>();

        CircleCollider2D* body1Collider = static_cast<CircleCollider2D*>(collider1);
        CircleCollider2D* body2Collider = static_cast<CircleCollider2D*>(collider2);

        float sumOfRaidus = body1Collider->GetRadius() + body2Collider->GetRadius();
        Vector2f relativePositionVector = 
            body1->GetGameObject()->GetComponent<Transform2D>()->GetPosition() -
            body2->GetGameObject()->GetComponent<Transform2D>()->GetPosition();

        float s = relativePositionVector.Magnitude() - sumOfRaidus;

        retval.m_collisionNormalVec = relativePositionVector.Normalize();

        Vector2f v1 = body1->GetVelocity();
        Vector2f v2 = body2->GetVelocity();
        retval.m_relativeVelocityVec = v1 - v2;

        float rvn = retval.m_collisionNormalVec.Dot(retval.m_relativeVelocityVec);

        // They are approaching each other
        if (s <= CollisionTolerance && rvn < 0.0f)
        {
            retval.m_collisionType = CollisionType::IsCollision;

            retval.m_collisionBody1 = body1;
            retval.m_collisionBody2 = body2;

        }
        else if (s < -CollisionTolerance)
        {
            retval.m_collisionType = CollisionType::Penetrating;
        }
        else
        {
            retval.m_collisionType = CollisionType::NoCollision;
        }

        return retval;
    }

    Physics2D::CollisionInfo Physics2D::CheckForCollisionCircleAndBox(Rigidbody2D* ball, Rigidbody2D* box)
    {

        CollisionInfo retval;

        CircleCollider2D* ballCollider = ball->GetGameObject()->GetComponent<CircleCollider2D>();
        BoxCollider2D* boxCollider = box->GetGameObject()->GetComponent<BoxCollider2D>();

        //radius
        float radius = ballCollider->GetRadius();

        //Quick test
        Vector2f circlePosition = ball->GetGameObject()->GetComponent<Transform2D>()->GetPosition();
        Vector2f boxPosition = box->GetGameObject()->GetComponent<Transform2D>()->GetPosition();
        Vector2f boxMinPoint(0, 0);
        Vector2f size = boxCollider->GetSize();

        boxMinPoint -= size / 2;

        /*
        Our code will first test which edge of the rectangle is closest to the circle,
        then see if there is a collision using the Pythagorean Theorem.
        Let’s create a temporary variable for the square’s closest X/Y edges.
        We’ll set them as the circle’s position to start:
        */
        Vector2f closestToCircle = circlePosition;
        if (closestToCircle.x < boxMinPoint.x) closestToCircle.x = boxMinPoint.x;
        if (closestToCircle.y < boxMinPoint.y) closestToCircle.y = boxMinPoint.y;
        if (closestToCircle.x > boxMinPoint.x + size.x) closestToCircle.x = boxMinPoint.x + size.x;
        if (closestToCircle.y > boxMinPoint.y + size.y) closestToCircle.y = boxMinPoint.y + size.y;

        /*
        Now that we know which edges to check,
        we run the Pythagorean Theorem code using the circle’s center and the two edges we found above:
        */
        float distX = circlePosition.x - closestToCircle.x;
        float distY = circlePosition.y - closestToCircle.y;
        float distance = sqrtf(distX * distX + distY * distY);
        float relativeDistance = distance - radius;

        retval.m_collisionNormalVec = (circlePosition - closestToCircle).Normalize();
        retval.m_relativeVelocityVec = ball->GetVelocity() - box->GetVelocity();
        float rvn = retval.m_collisionNormalVec.Dot(retval.m_relativeVelocityVec);

        if (relativeDistance < CollisionTolerance && rvn < 0.0f)
        {
            retval.m_collisionType = CollisionType::IsCollision;

            retval.m_collisionBody1 = ball;
            retval.m_collisionBody2 = box;
        }
        else if (relativeDistance < -CollisionTolerance)
        {
            retval.m_collisionType = CollisionType::Penetrating;
        }
        else
        {
            retval.m_collisionType = CollisionType::NoCollision;
        }

        return retval;

    }

    void Physics2D::ApplyImpulse( CollisionInfo info)
    {
        Rigidbody2D* body1 = info.m_collisionBody1;
        Rigidbody2D* body2 = info.m_collisionBody2;

        float j = (info.m_relativeVelocityVec.Dot(info.m_collisionNormalVec) *
            -(1 +  CoefficientOfRestitution)) /
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

    void Physics2D::ApplyImpulseStaic(CollisionInfo info)
    {
        Vector2f dynamicBodyVelocity;
        Rigidbody2D* staticBody  = info.m_collisionBody1;
        Rigidbody2D* dynamicBody  = info.m_collisionBody2;
        if (staticBody->GetIsStatic() == true)
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


}