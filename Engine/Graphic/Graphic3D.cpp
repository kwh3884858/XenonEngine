#include "Graphic3D.h"
#include "MathLab/Matrix.h"

#include <cassert>

//#include "MathLab/Vector2.h"

#include "Engine/GameObject.h"
#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Camera3D.h"
#include "Engine/Component/Mesh3D.h"

#include "CrossPlatform/Polygon3D.h"
namespace XenonEngine
{
	using MathLab::TMatrix4X3f;
	using MathLab::TVector4f;
    using MathLab::Vector2f;
    using CrossPlatform::Polygon3D;

    bool Graphic3D::Initialize()
    {
        m_viewDistance = Vector2f(1, 1);
    }

    void Graphic3D::AddGameobjectToRenderList(GameObject* gameobject)
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
            Mesh3D* mesh = iter->GetComponent<Mesh3D>();
            if (!mesh || !transform)
            {
                continue;
            }
            TMatrix4X4f localToWorldTransform = transform->GetLocalToWorldTransformMatrix();
            TMatrix4X4f worldToCameraTransform = GetMajorCamera()->GetCameraTransformInverseMatrix();
            //Culling
            //Remove back faces
            TMatrix4X4f perspectiveTransfrom = GetProjectileMatrix();
            TMatrix4X4f localToPerspectiveTranform = localToWorldTransform * worldToCameraTransform * perspectiveTransfrom;

            const Polygon3D* polygon = mesh->GetPolygon3D();
            for (int i = 0 ;i < polygon->Count(); i++)
            {
                const Vector3f& vertex = (*polygon)[i];
                TVector4f homogeneousVertex (vertex);
                homogeneousVertex = homogeneousVertex * localToPerspectiveTranform;
                Vector3f  ConvertFormHomogeneous(homogeneousVertex);

            }
		}
	}

    const XenonEngine::Camera3D* Graphic3D::GetMajorCamera() const
    {
        assert(m_cameraList != 0);
        return m_cameraList[0];
    }

    MathLab::TMatrix4X4f Graphic3D::GetProjectileMatrix() const
    {
        return TMatrix4X4f(
            std::initializer_list<float>{
            m_viewDistance.x, 0, 0, 0,
                0, m_viewDistance.y * m_aspectRatio, 0, 0,
                0, 0, 1, 1,
                0, 0, 0, 0
        });
    }

}