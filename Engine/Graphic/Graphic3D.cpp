#include "Graphic3D.h"
#include "MathLab/Matrix.h"

#include <cassert>

//#include "MathLab/Vector2.h"

#include "Engine/GameObject.h"
#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Camera3D.h"
#include "Engine/Component/LightComponent.h"

#include "Engine/Graphic/Graphic2D.h"

#include "CrossPlatform/Polygon3D.h"
#include <stdio.h> // for printf
#include <assert.h>
#include "../Component/DirectionLightComponent.h"
#include "../Component/PointLightComponent.h"

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

            //Remove back faces
            TMatrix4X4f cameraToProjectionTransfrom = GetProjectionMatrix(majorCamera->GetViewDistance(), majorCamera->GetAspectRatio());
            TMatrix4X4f projectionToScreenTransfrom = GetScreenMatrix(majorCamera->GetViewport());
            TMatrix4X4f cameraToScreenTranform = GetProjectionAndScreenMatrix(majorCamera->GetFov(), majorCamera->GetViewport());
            TMatrix4X4f worldToScreenTranform = worldToCameraTransform * cameraToScreenTranform;
            TMatrix4X4f localToScreenTranform = localToWorldTransform * worldToScreenTranform;
            TMatrix4X4f localToCameraTranform = localToWorldTransform * worldToCameraTransform;

            DrawCoordinateLines(worldToScreenTranform);

            const Polygon3D* polygon = mesh->GetPolygon3D();
            CullingState state = Culling(*mesh, localToCameraTranform, *majorCamera);
            if (state == CullingState::Culled)
            {
                continue;
            }

            for (int polyIndex = 0; polyIndex < polygon->Count(); polyIndex += 3)
            {
                const Vector3f& vertex0 = (*polygon)[polyIndex].m_vertex;
                const Vector3f& vertex1 = (*polygon)[polyIndex + 1].m_vertex;
                const Vector3f& vertex2 = (*polygon)[polyIndex + 2].m_vertex;
                TVector4f homogeneousVertex0 = ConvertFromNonHomogeneous(vertex0);
                TVector4f homogeneousVertex1 = ConvertFromNonHomogeneous(vertex1);
                TVector4f homogeneousVertex2 = ConvertFromNonHomogeneous(vertex2);
                homogeneousVertex0 = homogeneousVertex0 * localToCameraTranform;
                homogeneousVertex1 = homogeneousVertex1 * localToCameraTranform;
                homogeneousVertex2 = homogeneousVertex2 * localToCameraTranform;
                state = RemoveBackFaces(homogeneousVertex0, homogeneousVertex1, homogeneousVertex2);
                if (state == CullingState::Culled)
                {
                    continue;
                }
                //Lighting
                SColorRGBA baseColor = CrossPlatform::GERY;
                SColorRGBA finalColor;
                for (int i =0 ;i< m_lightList.Count(); i++)
                {
                    TVector4f zeroToOne = homogeneousVertex1 - homogeneousVertex0;
                    TVector4f zeroToTwo = homogeneousVertex2 - homogeneousVertex0;
                    TVector4f faceNormal = zeroToTwo.Cross(zeroToOne).Normalize();
                    assert(m_lightList[i]->GetLightType() != LightComponent::LightType::None);
                    if (m_lightList[i]->GetLightType() == LightComponent::LightType::Direction)
                    {
                        DirectionLightComponent* directionLight = static_cast<DirectionLightComponent*>(m_lightList[i]);
                        TVector4f direction = ConvertFromNonHomogeneous(directionLight->GetDirection());
                        float face = direction.Dot(faceNormal);
                        if (face > 0)
                        {
                            finalColor += directionLight->GetColor() * baseColor * face;
                        }
                    }
                    if (m_lightList[i]->GetLightType() == LightComponent::LightType::Point)
                    {
                        PointLightComponent* pointLight = static_cast<PointLightComponent*>(m_lightList[i]);
                        Vector3f lightPosition = pointLight->GetGameObject()->GetComponent<Transform3D>()->GetPosition();
                        TVector4f lightPoistionHomogeneous = MathLab::ConvertFromNonHomogeneous(lightPosition);
                        lightPoistionHomogeneous = lightPoistionHomogeneous * worldToCameraTransform;
                        lightPosition = MathLab::ConvertFormHomogeneous(lightPoistionHomogeneous);
                        TVector4f direction = vertex0 - lightPosition;
                        float kc = pointLight->GetKc();
                        float kl = pointLight->GetKl();
                        float face = direction.Normalize().Dot(faceNormal);
                        if (face > 0)
                        {
                            float attenuation = kc + kl * direction.Magnitude();
                            finalColor += pointLight->GetColor() * baseColor *  face / attenuation;
                        }
                    }
                }
                homogeneousVertex0 = homogeneousVertex0 * cameraToScreenTranform;
                Vector3f screenPosition1 = ConvertFormHomogeneous(homogeneousVertex0);
                Vector2f screenPoint1(screenPosition1.x, screenPosition1.y);

                homogeneousVertex1 = homogeneousVertex1 * cameraToScreenTranform;
                Vector3f screenPosition2 = ConvertFormHomogeneous(homogeneousVertex1);
                Vector2f screenPoint2(screenPosition2.x, screenPosition2.y);

                homogeneousVertex2 = homogeneousVertex2 * cameraToScreenTranform;
                Vector3f screenPosition3 = ConvertFormHomogeneous(homogeneousVertex2);
                Vector2f screenPoint3(screenPosition3.x, screenPosition3.y);


                if (m_renderType == RenderType::FlatShdering)
                {
                    //static int test = 0;
                    Graphic2D::Get().DrawTriangle(screenPoint1, screenPoint2, screenPoint3, finalColor);
                    //if (test < 1)
                    //{
                    //    printf("(%f, %f) -- (%f, %f) -- (%f, %f)\n", screenPoint1.x, screenPoint1.y, screenPoint2.x, screenPoint2.y, screenPoint3.x, screenPoint3.y);
                    //    test++;
                    //}
                }
                if (m_renderType == RenderType::Wireframe)
                {
                    Vector2f tmp0;
                    Vector2f tmp1;
                    tmp0 = screenPoint1;
                    tmp1 = screenPoint2;
                    Graphic2D::ClipLineState state = Graphic2D::Get().ClipLine(tmp0, tmp1);
                    if (state == Graphic2D::ClipLineState::Accpet)
                    {
                        Graphic2D::Get().DrawLine(tmp0, tmp1);
                    }
                    tmp0 = screenPoint2;
                    tmp1 = screenPoint3;
                    state = Graphic2D::Get().ClipLine(tmp0, tmp1);
                    if (state == Graphic2D::ClipLineState::Accpet)
                    {
                        Graphic2D::Get().DrawLine(tmp0, tmp1);
                    }
                    tmp0 = screenPoint3;
                    tmp1 = screenPoint1;
                    state = Graphic2D::Get().ClipLine(tmp0, tmp1);
                    if (state == Graphic2D::ClipLineState::Accpet)
                    {
                        Graphic2D::Get().DrawLine(tmp0, tmp1);
                    }
                }

            }
		}
	}

    XenonEngine::Graphic3D::CullingState Graphic3D::Culling(const Mesh3D& mesh, const TMatrix4X4f& localToCameraTranform, const Camera3D& camera) const
    {
        TVector4f origin;
        origin[3] = 1;
        origin = origin * localToCameraTranform;
        Vector3f center = ConvertFormHomogeneous(origin);
        float distance = camera.GetViewDistance();
        float maxRadius = mesh.GetMaxRadius();
        if (center.z + maxRadius < camera.GetNearClipZ() || center.z - maxRadius > camera.GetFarClipZ())
        {
            return CullingState::Culled;
        }
        float xLimit = 0.5f * camera.GetViewPlane().x * center.z / distance;
        if (center.x + maxRadius > xLimit || center.x - maxRadius < -xLimit)
        {
            return CullingState::Culled;
        }
        float yLimit = 0.5f * camera.GetViewPlane().y * center.z / distance;
        if (center.y + maxRadius > yLimit || center.y - maxRadius < -yLimit)
        {
            return CullingState::Culled;
        }
        return CullingState::Inside;
    }

    Graphic3D::CullingState Graphic3D::RemoveBackFaces(const TVector4f& p0, const TVector4f& p1, const TVector4f& p2) const
    {
        TVector4f u = p1 - p0;
        TVector4f v = p2 - p1;
        TVector4f n = u.Cross(v);

        if( p0.Dot(n) <= 0)
        {
            return CullingState::Inside;
        }
        else
        {
            return CullingState::Culled;
        }
    }

    void Graphic3D::DrawLine(const MathLab::Vector3f& start, const MathLab::Vector3f& end, const MathLab::TMatrix4X4f& localToScreenTranform, const CrossPlatform::SColorRGBA& rgba /*= CrossPlatform::WHITE*/) const
    {
        TVector4f homogeneousVertex1(start);
        homogeneousVertex1[3] = 1.0f;
        homogeneousVertex1 = homogeneousVertex1 * localToScreenTranform;
        Vector3f screenPosition1 = ConvertFormHomogeneous(homogeneousVertex1);
        Vector2f screenPoint1(screenPosition1.x, screenPosition1.y);

        TVector4f homogeneousVertex2(end);
        homogeneousVertex2[3] = 1.0f;
        homogeneousVertex2 = homogeneousVertex2 * localToScreenTranform;
        Vector3f screenPosition2 = ConvertFormHomogeneous(homogeneousVertex2);
        Vector2f screenPoint2(screenPosition2.x, screenPosition2.y);

        Graphic2D::ClipLineState state = Graphic2D::Get().ClipLine(screenPoint1, screenPoint2);
        if (state == Graphic2D::ClipLineState::Accpet)
        {
            Graphic2D::Get().DrawLine(screenPoint1, screenPoint2, rgba);
        }
    }

    void Graphic3D::DrawCoordinateLines(const TMatrix4X4f& worldToScreenTranform) const
    {
        Vector3f origin(0, 0, 0);
        Vector3f xAxisL(-10, 0, 0);
        Vector3f xAxisR(10, 0, 0);
        DrawLine(xAxisL, origin, worldToScreenTranform, CrossPlatform::INDIAN_RED);
        DrawLine(origin, xAxisR, worldToScreenTranform, CrossPlatform::RED);
        Vector3f yAxisU(0, 10, 0);
        Vector3f yAxisD(0, -10, 0);
        DrawLine(yAxisU, origin, worldToScreenTranform, CrossPlatform::GREEN);
        DrawLine(origin, yAxisD, worldToScreenTranform, CrossPlatform::DARK_GREEN);
        Vector3f zAxisF(0, 0, 10);
        Vector3f zAxisB(0, 0, -10);
        DrawLine(zAxisB, origin, worldToScreenTranform, CrossPlatform::DRAK_BLUE);
        DrawLine(origin, zAxisF, worldToScreenTranform, CrossPlatform::BLUE);
    }

    const XenonEngine::Camera3D* Graphic3D::GetMajorCamera() const
    {
        assert(m_cameraList.Count() != 0);
        return m_cameraList[0];
    }

    MathLab::TMatrix4X4f Graphic3D::GetProjectionMatrix(const float& viewDistance, float aspectRatio) const
    {
        assert(aspectRatio >= 1);
        return TMatrix4X4f(
            std::initializer_list<float>{
            viewDistance, 0, 0, 0,
                0, viewDistance * aspectRatio, 0, 0,
                0, 0, 1, 1,
                0, 0, 0, 0
        });
    }

    MathLab::TMatrix4X4f Graphic3D::GetScreenMatrix(const Vector2f& viewPort) const
    {
        float alpha = 0.5f * viewPort.x - 0.5;
        float beta = 0.5f * viewPort.y - 0.5;
        return TMatrix4X4f(
            std::initializer_list<float>{
                alpha, 0, 0, 0,
                0, beta, 0, 0,
                alpha, beta, 1, 0,
                0, 0, 0, 1
        });
    }

    MathLab::TMatrix4X4f Graphic3D::GetProjectionAndScreenMatrix(const float fov, const MathLab::Vector2f& viewPort) const
    {
        float viewDistance = tan(fov/2) * (viewPort.x / 2);
        float alpha = 0.5f * viewPort.x - 0.5;
        float beta = 0.5f * viewPort.y - 0.5;
        return TMatrix4X4f(
            std::initializer_list<float>{
                viewDistance, 0, 0, 0,
                0, viewDistance, 0, 0,
                alpha, beta, 1, 1,
                0, 0, 0, 0
        });
    }

}