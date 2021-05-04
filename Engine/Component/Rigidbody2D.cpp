#include "Rigidbody2D.h"
#include "Engine/Physics/Force2D.h"
namespace XenonEngine
{

    Rigidbody2D::Rigidbody2D()
    {
    }

    Rigidbody2D::~Rigidbody2D()
    {
    }

    bool Rigidbody2D::FixedUpdate(double deltaTime)
    {
        Vector3f a = Vector3f(0, 0, 0);
        Vector3f dv = Vector3f(0, 0, 0);
        Vector3f ds = Vector3f(0, 0, 0);

        float aa;
        float dav;
        float dr;

        Matrix3f rotationReverse = mTransform->getRotationInverseMatrixVec();

        calculateForces(deltaTime);

        //Integrate linear equation of motion
        a = mForces / mMass;

        //form local to world space
        dv = a * deltaTime;
        mVelocity += dv;

        ds = mVelocity * deltaTime;

        mTransform->addPosition(ds);

        //Calculate the angular velocity of the airplane in local space
        //angular acceleration
        //aa = mMoment.y() / mInertia;
        //Vector3f deltaAngularVelocity = mInertiaInverse * (mMoment - (mLocalAngularVelocity.cross(mInertia * mLocalAngularVelocity))) * deltaTime;

        //mLocalAngularVelocity += deltaAngularVelocity;

        Vector3f deltaAngularVelocity = mVelocity.cross(Vector3f(0, -1, 0)) / 10;

        mLocalAngularVelocity += deltaAngularVelocity;

        //Calculate the new rotation quaternion
        Vector3f deltaRotatation = (deltaAngularVelocity) * (0.5f * deltaTime);

        deltaRotatation.normalize();
        //dav = aa * deltaTime;

        /*float avY = XMVectorGetByIndex(mLocalAngularVelocity, 1) + dav;
        XMVectorSetByIndex(mLocalAngularVelocity, avY, 1);*/
        //mLocalAngularVelocity.y() += dav;

        //dr = mLocalAngularVelocity.y() * deltaTime * 57.29578f;
        mTransform->addRotation(deltaRotatation.x(), deltaRotatation.y(), deltaRotatation.z());

        //Misc. calculation
        //mSpeed = XMVectorGetByIndex(XMVector3Length(mVelocity), 0);
        mSpeed = mVelocity.norm();


        //mLocalVelocity = XMVector3TransformCoord(mVelocity, rotationReverse);
        mLocalVelocity = rotationReverse * mVelocity;

        return true;
    }

}