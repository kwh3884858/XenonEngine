#include "Engine/GameObject.h"
#include "Engine/Component/Rigidbody2D.h"
#include "Engine/Component/Transform2D.h"
#include "Engine/Component/Collider2D.h"

#include "Engine/Physics/Force2D.h"
#include "Engine/Physics/PhysicsConstant.h"

#include "MathLab/Vector3.h"
#include "MathLab/MathLib.h"
#include <cassert>
namespace XenonEngine
{
    using MathLab::Vector3;
    using XenonPhysics::Force2D;
    
    Rigidbody2D::Rigidbody2D(GameObject* gameobject, bool isStatic, float mass, float inertia) :
        IComponent(ComponentType::Rigidbody2D, gameobject),
        mIsStatic(isStatic),
        m_mass(mass),
        m_inertia(inertia),
        m_inertiaInverse(1/m_inertia),
        m_velocity(Vector2f::Zero),
        m_localVelocity(Vector2f::Zero),
        m_localAngularVelocity(0),
        m_speed(0),
        m_gravity(Vector2f(0, m_mass * XenonPhysics::Gravity)),
        m_forces(Vector2f::Zero),
        m_moments(0)
    {
    }

    Rigidbody2D::~Rigidbody2D()
    {
    }

    bool Rigidbody2D::FixedUpdate(float deltaTime)
    {
        CalculateForcesAndMoments(deltaTime);

        //Integrate linear equation of motion
        Vector2f a = m_forces / m_mass;

        //form local to world space
        Vector2f dv = a * deltaTime;
        m_velocity += dv;
        Vector2f ds = m_velocity * deltaTime;
        Transform2D* transform = m_gameobject->GetComponent<Transform2D>(ComponentType::Transform);
        assert(transform != nullptr);
        transform->AddPosition(ds);

        //Calculate the angular velocity of the airplane in local space
        //angular acceleration
        float aa = m_moments * m_inertiaInverse;
        float dav = aa * deltaTime;
        m_localAngularVelocity += dav;
        float dr = MathLab::DegreeToRadians(m_localAngularVelocity) * deltaTime ;
        transform->AddRotation(dr);

        //Misc. calculation
        m_speed = m_velocity.Magnitude();
        m_localVelocity = MathLab::Rotate2D(m_velocity, -transform->GetOrientation()); 

        //Reset force
        m_forces = Vector2f::Zero;
        m_moments = 0;

        return true;
    }

    bool Rigidbody2D::AddForce(const XenonPhysics::Force2D& force)
    {
        m_forces += force.m_forceDirection * force.fvalue;
        return true;
    }

    void Rigidbody2D::CalculateForcesAndMoments(double deltaTime)
    {
        assert(m_gameobject != nullptr);

        Vector2f sumOfForces = Vector2f::Zero;
        float sumOfMoments =0;

        //Calculate forces and momnents in body space

        //XMVECTOR vtmp;

        //Calculate the aerodynamic drag force
            //Calculate lcoal velocity
            //The local velocity includes the velocity due to 
            //linear motion of the craft,
            //plus the velocity at each element due to
            //the rotation of the craft
        float projectedArea = 1;
        float radius = 0;

        XenonEngine::Collider2D* collider = m_gameobject->GetComponent<XenonEngine::Collider2D>(ComponentType::Collider2D);
        if (collider != nullptr)
        {
            projectedArea = collider->GetArea();
            radius = collider->GetRadius();
        }

        Vector2f tagent (0, radius);

        float localSpeed = m_localVelocity.Magnitude();
        
        if (localSpeed > 0.0f)
        {
            Vector2f normalizedVelocity = m_velocity.Normalize();
            Vector2f dragVector = -normalizedVelocity;

            //Determine the resultant force on the element
            float tmp = 0.5f * XenonPhysics::AirDensity * localSpeed * localSpeed * projectedArea * XenonPhysics::LinearDragCofficient;
            Vector2f resultant = dragVector * tmp;

            //Keep a running total of these resultant forces
            sumOfForces += resultant;

            //Calculate the moment, keep a running total of these resultant moments
            sumOfMoments += tagent.Cross(resultant);
        }

        //I am not sure these codes whether should put in here
        //Calculate local angular velocity
        /*
       float localAngularSpeed = mLocalAngularVelocity.Magnitude();
        if (localAngularSpeed > 0.0f)
        {
            Vector2f normalizedLocalAngularVector = mLocalAngularVelocity.Normalize();
            Vector2f dragAngularVector = -normalizedLocalAngularVector;
            tmp = AirDensity * localAngularSpeed * localAngularSpeed * projectedArea * AngularDragCoefficient;

            Vector2f resultant = dragAngularVector * tmp;

            sumOfMoments += resultant;
        }
        */

        //Now add the propulsion thrust
        //No moment since line of action is through center of gravity
        sumOfForces += m_forces;

        Transform2D* transform = m_gameobject->GetComponent<Transform2D>(ComponentType::Transform);
        assert(transform != nullptr);
        //Convert forces from model spece to world space
        m_forces = MathLab::Rotate2D(sumOfForces, transform->GetOrientation());
        m_moments = sumOfMoments;
        
    }

}