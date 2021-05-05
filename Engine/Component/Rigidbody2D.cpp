#include "Rigidbody2D.h"
#include "Engine/Physics/Force2D.h"
#include "Engine/Physics/PhysicsConstant.h"
namespace XenonEngine
{

    Rigidbody2D::Rigidbody2D(GameObject* gameobject, bool isStatic, float mass, float inertia) :
        IComponent(ComponentType::Rigidbody2D, gameobject),
        mIsStatic(isStatic),
        mMass(mass),
        mInertia(inertia),
        mInertiaInverse(1/mInertia),
        mVelocity(Vector2f.Zero),
        mLocalVelocity(Vector2f.Zero),
        mLocalAngularVelocity(Vector2f.Zero),
        mSpeed(0),
        m_gravity(Vector2f(0, mMass * Gravity))
        m_forces(Vector2f.Zero),
        m_moment(Vector2f.Zero)
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

        CalculateForcesAndMoments(deltaTime);

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

        //Reset force
        m_forces = Vector2f.Zero;

        return true;
    }

    bool Rigidbody2D::AddForce(const Physics2D*const force)
    {
        m_forces += force;
    }

    void Rigidbody2D::CalculateForcesAndMoments(double deltaTime)
    {
        Vector2f Fb = Vector2f.Zero;
        Vector2f Mb = Vector2f.Zero;

        m_forces = Vector2f.Zero;
        m_moment = Vector2f.Zero;

        //Calculate forces and momnents in body space
        float localAngularSpeed;

        Vector3f dragVector;
        Vector3f dragAngularVector;
        float tmp;
        Vector3f resultant;
        //XMVECTOR vtmp;

        //Calculate the aerodynamic drag force
            //Calculate lcoal velocity
            //The local velocity includes the velocity due to 
            //linear motion of the craft,
            //plus the velocity at each element due to
            //the rotation of the craft

        float localSpeed = mLocalVelocity.Magnitude();
        float projectedArea = 1;

        if (m_gameobject != nullptr)
        {
            Collider2D* collider = m_gameobject->GetComponent(IComponent::Collider2D);
            if (collider != nullptr)
            {
                projectedArea = collider->GetArea();
            }
        }

        if (localSpeed > 0.0f)
        {
            Vector3f normalizedVelocity = mVelocity.Normalize();
            dragVector = -normalizedVelocity;

            //Determine the resultant force on the element
            tmp = 0.5f * AirDensity * localSpeed * localSpeed * projectedArea * LinearDragCofficient;

            resultant = dragVector * tmp;

            //Keep a running total of these resultant forces
            Fb += resultant;

        }

        //Calculate local angular velocity
        localAngularSpeed = mLocalAngularVelocity.norm();
        if (localAngularSpeed > 0.0f)
        {
            Vector3f normalizedLocalAngularVector = mLocalAngularVelocity.normalized();
            dragAngularVector = -normalizedLocalAngularVector;
            tmp = AirDensity * localAngularSpeed * localAngularSpeed * projectedArea * AngularDragCoefficient;

            resultant = dragAngularVector * tmp;

            Mb += resultant;
        }
        //Now add the propulsion thrust
        //No moment since line of action is through center of gravity
        Fb += thrust;
        /*Mb += thrust.cos()*/

        //Convert forces from model spece to world space
        Matrix3f rotation = mTransform->getRotationMatrixVec();
        Matrix3f reverseRotation = mTransform->getRotationInverseMatrixVec();

        Mb += (reverseRotation * thrust).cross(reverseRotation * Vector3f(0, -1, 0));
        mForces = Fb;
        mMoment = Mb;

        //mMoment = Mb; // mb


        
    }

}