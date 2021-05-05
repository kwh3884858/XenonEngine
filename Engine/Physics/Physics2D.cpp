#include "Physics2D.h"
#include "BoxCollider2D.h"

namespace XenonPhysics
{
    using XenonEngine::Collider2D;
    Physics2D::Physics2D()
    {
    }

    Physics2D::~Physics2D()
    {
    }

    bool Physics2D::FixedUpdate()
    {
        bool result;
        float deltaTime = TIMESTEP;
        bool tryAgain = true;
        CollisionType collisionType = CollisionType::NoCollision;
        Rigidbody2D body1, body2;
        bool penetrated = false;
        int count = 0;

        //Delta time must bigger than 0
        if (deltaTime <= EPSILON)
        {
            return true;
        }

        //Update position
        for (size_t i = 0; i < mCount; i++)
        {
            mDynamicRigidbodys[i]->FixedUpdate(deltaTime);
        }

        //Check collision
        for (size_t i = 0; i < mCount - 1; i++) {


            for (size_t j = i + 1; j < mCount; j++)
            {

                while (tryAgain && deltaTime > MINDELTATIME)
                {
                    tryAgain = false;
                    memcpy(&body1, mDynamicRigidbodys[i], sizeof(Rigidbody2D));
                    memcpy(&body2, mDynamicRigidbodys[j], sizeof(Rigidbody2D));

                    //body1.fixedUpdate(deltaTime);
                    //body2.fixedUpdate(deltaTime);

                    mCollisionBody1 = nullptr;
                    mCollisionBody2 = nullptr;


                    collisionType = CheckForCollision(&body1, &body2);

                    if (collisionType == CollisionType::Penetrating)
                    {
                        deltaTime /= 2;
                        tryAgain = true;
                        penetrated = true;
                    }
                    else if (collisionType == CollisionType::IsCollision)
                    {
                        if (mCollisionBody1 != nullptr && mCollisionBody2 != nullptr)
                        {
                            ApplyImpulse(mCollisionBody1, mCollisionBody2);
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

        //for static
        for (size_t i = 0; i < mStaticRigidbodys.count(); i++)
        {
            for (size_t j = 0; j < mCount; j++)
            {
                while (tryAgain && deltaTime > MINDELTATIME)
                {
                    tryAgain = false;
                    memcpy(&body1, mStaticRigidbodys[i], sizeof(Rigidbody2D));
                    memcpy(&body2, mDynamicRigidbodys[j], sizeof(Rigidbody2D));

                    body1.fixedUpdate(deltaTime);
                    body2.fixedUpdate(deltaTime);

                    mCollisionBody1 = nullptr;
                    mCollisionBody2 = nullptr;

                    collisionType = CheckForCollision(&body1, &body2);

                    if (collisionType == CollisionType::Penetrating)
                    {
                        deltaTime /= 2;
                        tryAgain = true;
                        penetrated = true;
                    }
                    else if (collisionType == CollisionType::IsCollision)
                    {
                        if (mCollisionBody1 != nullptr && mCollisionBody2 != nullptr)
                        {
                            ApplyImpulseStaic(mCollisionBody1, mCollisionBody2);
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

}