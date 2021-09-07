#include "Graphic3D.h"
#include "MathLab/Matrix.h"

#include "Engine/Component/Transform3D.h"

namespace XenonEngine
{
	using MathLab::TMatrix4X3f;

	void Graphic3D::AddGameobjectToRenderList(const GameObject* gameobject)
	{
		m_renderList.Add(gameobject);
	}

	void Graphic3D::RemoveGameobjectFromRenderList(const GameObject* gameobject)
	{
		m_renderList.Remove(gameobject);
	}

	void Graphic3D::Render() const
	{
		for (int i = 0; i < m_renderList;i++)
		{
			GameObject* iter = m_renderList[i];
			Transform3D* transform = iter->GetComponent<Transform3D>();
			TMatrix4X3f localToWorldTransform = transform->get

		}
	}
}