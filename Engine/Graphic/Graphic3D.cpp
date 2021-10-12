#include "Graphic3D.h"
#include "MathLab/Matrix.h"

#include <cassert>

//#include "MathLab/Vector2.h"

#include "Engine/GameObject.h"
#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Camera3D.h"
#include "Engine/Component/Mesh3D.h"
#include "Engine/Graphic/Graphic2D.h"

#include "CrossPlatform/Polygon3D.h"
#include "CrossPlatform/Database.h"
namespace XenonEngine
{
	using MathLab::TMatrix4X3f;
	using MathLab::TVector4f;
    using MathLab::Vector2f;
    using CrossPlatform::Polygon3D;
    using CrossPlatform::Database;

    bool Graphic3D::Initialize()
    {
        m_viewDistance = Vector2f(1, 1);
        return true;
    }

    void Graphic3D::AddGameobjectToRenderList(GameObject* gameobject)
	{
		m_renderList.Add(gameobject);
	}

	void Graphic3D::RemoveGameobjectFromRenderList( GameObject* gameobject)
	{
		m_renderList.Remove(gameobject);
	}

	void Graphic3D::Update() const
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
            TMatrix4X4f cameraToProjectionTransfrom = GetProjectionMatrix();
            TMatrix4X4f projectionToScreenTransfrom = GetScreenMatrix();
            TMatrix4X4f localToScreenTranform = localToWorldTransform * worldToCameraTransform * cameraToProjectionTransfrom * projectionToScreenTransfrom;

            const Polygon3D* polygon = mesh->GetPolygon3D();
            for (int i = 0 ;i < polygon->Count(); i+=2)
            {
                const Vector3f& vertex1 = (*polygon)[i];
                TVector4f homogeneousVertex1 (vertex1);
                homogeneousVertex1 = homogeneousVertex1 * localToScreenTranform;
                Vector3f screenPosition1 = ConvertFormHomogeneous(homogeneousVertex1);
                Vector2i screenPoint1(screenPosition1.x, screenPosition1.y);

                const Vector3f& vertex2 = (*polygon)[i + 1];
                TVector4f homogeneousVertex2(vertex2);
                homogeneousVertex2 = homogeneousVertex2 * localToScreenTranform;
                Vector3f screenPosition2 = ConvertFormHomogeneous(homogeneousVertex2);
                Vector2i screenPoin2(screenPosition2.x, screenPosition2.y);
                
                Graphic2D::Get().DrawLine(screenPoint1, screenPoin2);
            }
		}
	}

    const XenonEngine::Camera3D* Graphic3D::GetMajorCamera() const
    {
        assert(m_cameraList.Count() != 0);
        return m_cameraList[0];
    }

    MathLab::TMatrix4X4f Graphic3D::GetProjectionMatrix() const
    {
        return TMatrix4X4f(
            std::initializer_list<float>{
                m_viewDistance.x, 0, 0, 0,
                0, m_viewDistance.y * m_aspectRatio, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
        });
    }

    MathLab::TMatrix4X4f Graphic3D::GetScreenMatrix() const
    {
        float alpha = 0.5f * Database::Get().engineConfig.m_width - 0.5;
        float beta = 0.5f * Database::Get().engineConfig.m_height - 0.5;
        return TMatrix4X4f(
            std::initializer_list<float>{
                alpha, 0, 0, 0,
                0, -beta, 0, 0,
                0, 0, 1, 1,
                alpha, beta, 0, 0
        });
    }

}