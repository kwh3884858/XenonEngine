#include "Camera3D.h"
#include "../Graphic/Graphic3D.h"

#include "Engine/Component/Transform3D.h"
#include "Engine/GameObject.h"

namespace XenonEngine
{
	using MathLab::Vector3f;
	using MathLab::TMatrix4X4f;

	IComponent* Camera3D::Copy(GameObject*const gameObject) const
	{
		Camera3D* that = new Camera3D(gameObject);
		that->m_lookAt = m_lookAt;
		that->m_viewDistance = m_viewDistance;
		that->m_fov = m_fov;
		that->m_viewport = m_viewport;
		return that;
	}

    bool Camera3D::Start()
    {
        Graphic3D::Get().AddCamera(this);
        return true;
    }

    bool Camera3D::Destroy()
    {
        Graphic3D::Get().RemoveCamera(this);
        return true;
    }

    void Camera3D::SetConfig(const Camera3DConfig*const config)
    {
        m_lookAt = config->m_lookAt;
        m_viewDistance = config->m_viewDistance;
        m_fov = config->m_fov;
        m_viewport = config->m_viewport;
    }

    MathLab::TMatrix4X4f Camera3D::GetCameraTransformInverseMatrix() const
	{
        Transform3D* tranform3D = GetGameObject()->GetComponent<Transform3D>();
        Vector3f position = tranform3D->GetPosition();

        // n coordinate = normal vector
        Vector3f lookAt = m_lookAt.Normalize();
		// u coordinate = right vector
		Vector3f rightVector = lookAt.Cross(Vector3f(0, 1, 0));
		// v coordinate = up vector
		Vector3f upVector = rightVector.Cross(lookAt);
		return TMatrix4X4f(
			std::initializer_list<float>{
				rightVector.x, upVector.x, lookAt.x,0,
				rightVector.y, upVector.y, lookAt.y,0,
				rightVector.z, upVector.z, lookAt.z,0,
				-position.dot(rightVector), -position.dot(upVector), -position.dot(lookAt),1
		});
	}

    ComponentType Camera3D::m_type = ComponentType::ComponentType_Camera;
}