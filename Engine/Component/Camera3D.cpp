#include "Camera3D.h"
#include "../Graphic/Graphic3D.h"

#include "Engine/Component/Transform3D.h"
#include "Engine/GameObject.h"
#include <cmath>

namespace XenonEngine
{
	using MathLab::Vector2f;
	using MathLab::Vector3f;
	using MathLab::TMatrix4X4f;

	IComponent* Camera3D::Copy(GameObject*const gameObject) const
	{
		Camera3D* that = new Camera3D(gameObject);
		that->m_lookAt = m_lookAt;
		that->m_fov = m_fov;
		that->m_viewPlane = m_viewPlane;
		that->m_viewport = m_viewport;
        that->m_farClipZ = m_farClipZ;
        that->m_nearClipZ = m_nearClipZ;
		return that;
	}

    bool Camera3D::Start()
    {
        Graphic3D::Get().AddCamera(this);
        return true;
    }

    bool Camera3D::Update()
    {
        const Transform3D* tranform3D = GetGameObject()->GetComponentPointer<Transform3D>();
        Vector3f rotation = tranform3D->GetRotation();
        SetEularLookAt(Vector2f(rotation.y, rotation.x));
        return true;
    }

    bool Camera3D::Destroy()
    {
        Graphic3D::Get().RemoveCamera(this);
        return true;
    }

    void Camera3D::SetConfig(const Camera3DConfig*const config)
    {
        assert(config->m_fov > 0);
        m_fov = config->m_fov;
        assert(config->m_viewport.x > 0 && config->m_viewport.y > 0);
        m_viewport = config->m_viewport;
        m_viewPlane = Vector2f(2, 2 * (m_viewport.x / m_viewport.y));
        m_nearClipZ = config->m_nearClipZ;
        m_farClipZ = config->m_farClipZ;
    }

    float Camera3D::GetViewDistance() const
    {
        float radius = MathLab::DegreeToRadians(m_fov / 2);
        return tan(radius) *(m_viewPlane.x / 2);
    }

    MathLab::TMatrix4X4f Camera3D::GetCameraTransformInverseMatrix() const
	{
        Transform3D* tranform3D = GetGameObject()->GetComponentPointer<Transform3D>();
        Vector3f position = tranform3D->GetPosition();

        // n coordinate = normal vector
        Vector3f lookAt = m_lookAt.Normalize();
		// u coordinate = right vector
 		Vector3f rightVector = Vector3f(0, 1, 0).Cross(lookAt);
		// v coordinate = up vector
		Vector3f upVector = lookAt.Cross(rightVector);
		return TMatrix4X4f(
			std::initializer_list<float>{
				rightVector.x, upVector.x, lookAt.x,0,
				rightVector.y, upVector.y, lookAt.y,0,
				rightVector.z, upVector.z, lookAt.z,0,
				-position.Dot(rightVector), -position.Dot(upVector), -position.Dot(lookAt),1
		});
	}

    void Camera3D::SetEularLookAt(const MathLab::Vector2f& headingAndElevation)
    {
        float headingRadius = MathLab::DegreeToRadians(headingAndElevation.x);
        float elevationRadius = MathLab::DegreeToRadians(headingAndElevation.y);

        m_lookAt.x = cos(elevationRadius) * cos(headingRadius);
        m_lookAt.y = sin(elevationRadius);
        m_lookAt.z = -cos(elevationRadius) * sin(headingRadius);
    }

    void Camera3D::SetLookAt(const MathLab::Vector3f& lookat)
    {
        float heading = MathLab::RadiansToDegree(atan(lookat.z / lookat.x));
        float elevation = MathLab::RadiansToDegree(asin(lookat.y));
        Transform3D* tranform3D = GetGameObject()->GetComponentPointer<Transform3D>();
        tranform3D->SetRotation(Vector3f(elevation, heading, 0));
    }

    ComponentType Camera3D::m_type = ComponentType::ComponentType_Camera;
}