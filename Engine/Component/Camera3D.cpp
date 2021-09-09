#include "Camera3D.h"

namespace XenonEngine
{
	using MathLab::Vector3f;
	using MathLab::TMatrix4X4f;

	IComponent* Camera3D::Copy(GameObject*const gameObject) const
	{
		Camera3D* that = new Camera3D(gameObject);
		that->m_position = m_position;
		that->m_lookAt = m_lookAt;
		that->m_viewDistance = m_viewDistance;
		that->m_fov = m_fov;
		that->m_viewport = m_viewport;
		return that;
	}

	MathLab::TMatrix4X4f Camera3D::GetCameraTransformInverseMatrix() const
	{
		// u coordinate = right vector
		Vector3f rightVector = m_lookAt.Cross(Vector3f(0, 1, 0));
		// v coordinate = up vector
		Vector3f upVector = rightVector.Cross(m_lookAt);
		// n coordinate = normal vector
		return TMatrix4X4f(
			std::initializer_list<float>{
				rightVector.x, upVector.x, m_lookAt.x,0,
				rightVector.y, upVector.y, m_lookAt.y,0,
				rightVector.z, upVector.z, m_lookAt.z,0,
				-m_position.dot(rightVector), -m_position.dot(upVector), -m_position.dot(m_lookAt),1
		});
	}

    ComponentType Camera3D::m_type = ComponentType::ComponentType_Camera;
}