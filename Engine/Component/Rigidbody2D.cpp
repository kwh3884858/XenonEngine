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
        IComponent(gameobject),
        mIsStatic(isStatic),
        m_mass(mass),
        m_inertia(inertia),
        m_inertiaInverse(1/m_inertia)
    {
    }

    Rigidbody2D::~Rigidbody2D()
    {
    }

    const Rigidbody2D::FixedUpdateData& Rigidbody2D::PreFixedUpdate(float deltaTime)
    {
        FixedUpdateData newUpdateData;
        // When collision is happen, only accept collision impulse.
        //if (m_isCollision)
        //{
        //    m_isCollision = false;
        //    newUpdateData.m_forces = m_currentData.m_forces;
        //    newUpdateData.m_moments = m_currentData.m_moments;
        //}
        //else
        //{
        newUpdateData.m_addedForces = m_addedForces;
        m_addedForces = Vector2f::Zero;
        newUpdateData.m_addedMoments = m_addedMoments;
        m_addedMoments = 0;
        CalculateForcesAndMoments(newUpdateData, deltaTime);

        m_lastFrameFixedUpdateData = m_currentData;
        m_currentData = newUpdateData;
        m_isAllowPullBack = true;

        return m_currentData;
        //}
    }

    bool Rigidbody2D::FixedUpdate(float deltaTime)
    {
        if (mIsStatic)
        {
            return true;
        }
        CalculateRigidbodyByUpdateData(m_currentData, deltaTime);
        AffectGameobjectInWorld(m_currentData);
        return true;
    }

    bool Rigidbody2D::FallbackUpdate()
    {
        if (!m_isAllowPullBack)
        {
            return false;
        }
        Transform2D* transform = m_gameobject->GetComponent<Transform2D>();
        assert(transform != nullptr);
        transform->AddPosition(-m_currentData.m_deltaDistance);
        transform->AddRotation(-m_currentData.m_deltaRotation);

        m_currentData = m_lastFrameFixedUpdateData;
        m_isAllowPullBack = false;

        //Reset force
        m_addedForces = m_currentData.m_addedForces;
        m_addedMoments = m_currentData.m_addedMoments;

        return true;
    }

    bool Rigidbody2D::AddForce(const XenonPhysics::Force2D& force)
    {
        if (mIsStatic)
        {
            assert(true == false);
            return true;
        }
        Vector2f forceVector = force.m_forceDirection * force.fvalue;
        
        return AddForce(forceVector);
    }

    bool Rigidbody2D::AddLocalForce(const XenonPhysics::Force2D& force)
    {        
        //Convert forces from model spece to world space
        Transform2D* transform = m_gameobject->GetComponent<Transform2D>();
        assert(transform != nullptr);
        Vector2f localCoordinateForce = force.m_forceDirection * force.fvalue;
        Vector2f globalForce = MathLab::Rotate2D(localCoordinateForce, transform->GetOrientation());
        
        return AddForce(globalForce);
    }
    
    bool Rigidbody2D::AddForce(const Vector2f& force)
    {
        m_addedForces += force;
        return true;
    }

    Rigidbody2D::FixedUpdateData& Rigidbody2D::CalculateForcesAndMoments(FixedUpdateData& currentData, double deltaTime)
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
        float localSpeed = m_currentData.m_localVelocity.Magnitude();
        if (m_isEnableAirDrag && localSpeed > 0.0f)
        {
            float projectedArea = 1;
            float radius = 0;
            XenonEngine::Collider2D* collider = m_gameobject->GetComponent<XenonEngine::Collider2D>();
            if (collider != nullptr)
            {
                projectedArea = collider->GetArea();
                radius = collider->GetRadius();
            }
            Vector2f tagent(0, radius);
            Vector2f normalizedVelocity = m_currentData.m_velocity.Normalize();
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

        if (m_isSimulateGravity)
        {
            sumOfForces += Vector2f(0, m_mass * XenonPhysics::Gravity);
        }
        //Now add the propulsion thrust
        //No moment since line of action is through center of gravity
        currentData.m_sumOfForces = currentData.m_addedForces + sumOfForces;
        currentData.m_sumOfMoments = currentData.m_addedMoments + sumOfMoments;
        return currentData;
    }

    void Rigidbody2D::CalculateRigidbodyByUpdateData( FixedUpdateData& currentData, double deltaTime) const
    {
        //Integrate linear equation of motion
        Vector2f acceleration = currentData.m_sumOfForces / m_mass;
        //form local to world space
        Vector2f dv = acceleration * deltaTime;
        currentData.m_velocity = m_lastFrameFixedUpdateData.m_velocity + dv;
        currentData.m_deltaDistance = currentData.m_velocity * deltaTime;

        //Calculate the angular velocity of the airplane in local space
        //angular acceleration
        float aa = currentData.m_sumOfMoments * m_inertiaInverse;
        float dav = aa * deltaTime;
        currentData.m_localAngularVelocity = m_lastFrameFixedUpdateData.m_localAngularVelocity + dav;
        currentData.m_deltaRotation = MathLab::DegreeToRadians(currentData.m_localAngularVelocity) * deltaTime;

        //Misc. calculation
        Transform2D* transform = m_gameobject->GetComponent<Transform2D>();
        assert(transform != nullptr);
        currentData.m_speed = currentData.m_velocity.Magnitude();
        currentData.m_localVelocity = MathLab::Rotate2D(currentData.m_velocity, -(transform->GetOrientation() + currentData.m_deltaRotation));

    }

    void Rigidbody2D::AffectGameobjectInWorld(const FixedUpdateData& data)
    {
        Transform2D* transform = m_gameobject->GetComponent<Transform2D>();
        assert(transform != nullptr);
        transform->AddPosition(data.m_deltaDistance);
        transform->AddRotation(data.m_deltaRotation);

        //m_lastFrameFixedUpdateData = m_currentData;
        //m_currentData = data;
        //m_isAllowPullBack = true;
    }

    XenonEngine::IComponent* Rigidbody2D::Copy(GameObject*const gameObject) const
    {
        Rigidbody2D* that = new Rigidbody2D(gameObject, mIsStatic,m_mass, m_inertia);
        that->m_isEnableAirDrag = m_isEnableAirDrag;
        that->mIsModified = mIsModified;
        that->m_isSimulateGravity = m_isSimulateGravity;

        that->m_addedForces = m_addedForces;
        that->m_addedMoments = m_addedMoments;
        return that;
    }

    ComponentType Rigidbody2D::m_type = ComponentType::ComponentType_Rigidbody2D;

}