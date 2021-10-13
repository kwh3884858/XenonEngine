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
#include <stdio.h>

namespace XenonEngine
{
	using MathLab::TMatrix4X3f;
	using MathLab::TVector4f;
    using MathLab::Vector2f;
    using CrossPlatform::Polygon3D;

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
        const Camera3D* majorCamera = GetMajorCamera();
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
            TMatrix4X4f worldToCameraTransform = majorCamera->GetCameraTransformInverseMatrix();
            //Culling
            //Remove back faces
            TMatrix4X4f cameraToProjectionTransfrom = GetProjectionMatrix(majorCamera->GetViewDistance(), majorCamera->GetAspectRatio());
            TMatrix4X4f projectionToScreenTransfrom = GetScreenMatrix(majorCamera->GetViewport());
            TMatrix4X4f localToScreenTranform = localToWorldTransform * worldToCameraTransform * cameraToProjectionTransfrom * projectionToScreenTransfrom;

            const Polygon3D* polygon = mesh->GetPolygon3D();
            for (int i = 0; i < polygon->Count(); i += 3)
            {
                const Vector3f& vertex1 = (*polygon)[i];
                TVector4f homogeneousVertex1(vertex1);
                homogeneousVertex1[3] = 1.0f;
                homogeneousVertex1 = homogeneousVertex1 * localToScreenTranform;
                Vector3f screenPosition1 = ConvertFormHomogeneous(homogeneousVertex1);
                Vector2f screenPoint1(screenPosition1.x, screenPosition1.y);

                const Vector3f& vertex2 = (*polygon)[i + 1];
                TVector4f homogeneousVertex2(vertex2);
                homogeneousVertex2[3] = 1.0f;
                homogeneousVertex2 = homogeneousVertex2 * localToScreenTranform;
                Vector3f screenPosition2 = ConvertFormHomogeneous(homogeneousVertex2);
                Vector2f screenPoint2(screenPosition2.x, screenPosition2.y);

                const Vector3f& vertex3 = (*polygon)[i + 2];
                TVector4f homogeneousVertex3(vertex3);
                homogeneousVertex3[3] = 1.0f;
                homogeneousVertex3 = homogeneousVertex3 * localToScreenTranform;
                Vector3f screenPosition3 = ConvertFormHomogeneous(homogeneousVertex3);
                Vector2f screenPoint3(screenPosition3.x, screenPosition3.y);

                static int test = 0;
                //Graphic2D::Get().DrawTriangle(screenPoint1, screenPoint2, screenPoint3);
                //if (test < 1)
                //{
                //    printf("(%f, %f) -- (%f, %f) -- (%f, %f)\n", screenPoint1.x, screenPoint1.y, screenPoint2.x, screenPoint2.y, screenPoint3.x, screenPoint3.y);
                //    test++;
                //}

                Vector2f tmp0;
                Vector2f tmp1;
                tmp0 = screenPoint1;
                tmp1 = screenPoint2;
                Graphic2D::Get().ClipLine(tmp0, tmp1);
                Graphic2D::Get().DrawLine(tmp0, tmp1);
                tmp0 = screenPoint2;
                tmp1 = screenPoint3;
                Graphic2D::Get().ClipLine(tmp0, tmp1);
                Graphic2D::Get().DrawLine(tmp0, tmp1);
                tmp0 = screenPoint2;
                tmp1 = screenPoint3;
                Graphic2D::Get().ClipLine(tmp0, tmp1);
                Graphic2D::Get().DrawLine(tmp0, tmp1);
            }
		}
	}

    const XenonEngine::Camera3D* Graphic3D::GetMajorCamera() const
    {
        assert(m_cameraList.Count() != 0);
        return m_cameraList[0];
    }

    MathLab::TMatrix4X4f Graphic3D::GetProjectionMatrix(const Vector2f& viewDistance, float aspectRatio) const
    {
        assert(aspectRatio >= 1);
        return TMatrix4X4f(
            std::initializer_list<float>{
            viewDistance.x, 0, 0, 0,
                0, viewDistance.y * aspectRatio, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
        });
    }

    MathLab::TMatrix4X4f Graphic3D::GetScreenMatrix(const Vector2f& viewPort) const
    {
        float alpha = 0.5f * viewPort.x - 0.5;
        float beta = 0.5f * viewPort.y - 0.5;
        return TMatrix4X4f(
            std::initializer_list<float>{
                alpha, 0, 0, 0,
                0, -beta, 0, 0,
                0, 0, 1, 1,
                alpha, beta, 0, 0
        });
    }

}