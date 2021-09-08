#include "Graphic3D.h"
#include "MathLab/Matrix.h"

#include <cassert>

#include "Engine/GameObject.h"
#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Camera3D.h"

namespace XenonEngine
{
	using MathLab::TMatrix4X3f;

	void Graphic3D::AddGameobjectToRenderList( GameObject* gameobject)
	{
		m_renderList.Add(gameobject);
	}

	void Graphic3D::RemoveGameobjectFromRenderList( GameObject* gameobject)
	{
		m_renderList.Remove(gameobject);
	}

	void Graphic3D::Render() const
	{
		for (int i = 0; i < m_renderList.Count(); i++)
		{
			GameObject* iter = m_renderList[i];
			Transform3D* transform = iter->GetComponent<Transform3D>();
            TMatrix4X4f localToWorldTransform = transform->GetLocalToWorldTransformMatrix();
            TMatrix4X4f worldToCameraTransform = GetMajorCamera()->GetCameraTransformInverseMatrix();

		}
	}

    const XenonEngine::Camera3D* Graphic3D::GetMajorCamera() const
    {
        assert(m_cameraList != 0);
        return m_cameraList[0];
    }

}