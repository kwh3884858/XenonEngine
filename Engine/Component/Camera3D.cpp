#include "Camera3D.h"

namespace XenonEngine
{
	using MathLab::Vector3f;
	using MathLab::TMatrix4X3f;

	IComponent* Camera3D::Copy(GameObject*const gameObject) const
	{

	}

	MathLab::TMatrix4X3f Camera3D::GetCameraTransform() const
	{
		// u coordinate = right vector
		Vector3f rightVector = m_lookAt.Cross(Vector3f(0, 1, 0));
		// v coordinate = up vector
		Vector3f upVector = rightVector.Cross(m_lookAt);
		// n coordinate = normal vector
		return TMatrix4X3f(
			std::initializer_list<float>{
				rightVector.x, upVector.x, m_lookAt.x,
				rightVector.y, upVector.y, m_lookAt.y,
				rightVector.z, upVector.z, m_lookAt.z,
				-m_position.dot(rightVector), -m_position.dot(upVector), -m_position.dot(m_lookAt)
		});
	}

	ComponentType Camera3D::m_type = ComponentType::ComponentType_Camera;

}