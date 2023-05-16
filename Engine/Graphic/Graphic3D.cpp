#include "Graphic3D.h"
#include "MathLab/Matrix.h"

#include <cassert>

#include "Engine/GameObject.h"
#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Camera3D.h"
#include "Engine/Component/LightComponent.h"
#include "Engine/Component/DirectionLightComponent.h"
#include "Engine/Component/PointLightComponent.h"

#include "Engine/Graphic/Graphic2D.h"
#include "Engine/EngineManager.h"
#include "Engine/GameObjectWorld.h"

#include "CrossPlatform/Polygon/Polygon3D.h"
#include "CrossPlatform/Material/Material.h"

#include <stdio.h> // for printf
#include <assert.h>

#include "Algorithms/Sort.h"

namespace XenonEngine
{
	using MathLab::TMatrix4X3f;
	using MathLab::TVector4f;
    using MathLab::Vector2f;
    using MathLab::Vector3f;
	using namespace CrossPlatform;

    bool Graphic3D::VertexShaderFlat(const VertexShaderDataInputFlat& input, VertexShaderDataOutputFlat& output, const TMatrix4X4f& worldToCameraTransform, const TMatrix4X4f& cameraToScreenTranform) const
    {
        TVector4f homogeneousVertex0 = input.m_triangle[0].m_vertex;
        TVector4f homogeneousVertex1 = input.m_triangle[1].m_vertex;
        TVector4f homogeneousVertex2 = input.m_triangle[2].m_vertex;

        //Lighting
        SColorRGBA baseColor = input.m_faceColor;
        SColorRGBA finalColor = CrossPlatform::BLACK;

        TMatrix4X4f worldToCameraRotationMatrix = MathLab::GetRotationFromTransformMatrix(worldToCameraTransform);

        for (int i = 0; i < m_lightList.Count(); i++)
        {
            TVector4f zeroToOne = homogeneousVertex1 - homogeneousVertex0;
            TVector4f zeroToTwo = homogeneousVertex2 - homogeneousVertex0;
            TVector4f faceNormal = zeroToTwo.Cross(zeroToOne).Normalize(); //anti-lighting direction
            assert(m_lightList[i]->GetLightType() != LightComponent::LightType::None);
            if (m_lightList[i]->GetLightType() == LightComponent::LightType::Direction)
            {
                DirectionLightComponent* directionLight = static_cast<DirectionLightComponent*>(m_lightList[i]);
                Vector3f direction = directionLight->GetDirection();
                TVector4f tDirection = ConvertFromNonHomogeneous(direction);
                tDirection = tDirection * worldToCameraRotationMatrix;
                float face = tDirection.Dot(faceNormal);
                if (face > 0)
                {
                    finalColor += directionLight->GetColor() * baseColor * face;
                }
            }
            if (m_lightList[i]->GetLightType() == LightComponent::LightType::Point)
            {
                PointLightComponent* pointLight = static_cast<PointLightComponent*>(m_lightList[i]);
                Vector3f lightPosition = pointLight->GetGameObject()->GetComponentPointer<Transform3D>()->GetPosition();
                TVector4f lightPoistionHomogeneous = MathLab::ConvertFromNonHomogeneous(lightPosition);
                lightPoistionHomogeneous = lightPoistionHomogeneous * worldToCameraTransform;
                TVector4f direction = lightPoistionHomogeneous - homogeneousVertex0 ;
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
        output.m_screenPoint0 = Vector2f(screenPosition1.x, screenPosition1.y);

        homogeneousVertex1 = homogeneousVertex1 * cameraToScreenTranform;
        Vector3f screenPosition2 = ConvertFormHomogeneous(homogeneousVertex1);
        output.m_screenPoint1 = Vector2f(screenPosition2.x, screenPosition2.y);

        homogeneousVertex2 = homogeneousVertex2 * cameraToScreenTranform;
        Vector3f screenPosition3 = ConvertFormHomogeneous(homogeneousVertex2);
        output.m_screenPoint2 = Vector2f(screenPosition3.x, screenPosition3.y);

        output.m_faceColor = finalColor;
        return true;
    }

    bool Graphic3D::VertexShaderGouraud(const VertexShaderDataInputGouraud& input, VertexShaderDataOutputGouraud& output, const MathLab::TMatrix4X4f& worldToCameraTransform, const MathLab::TMatrix4X4f& cameraToScreenTranform) const
    {
        TMatrix4X4f worldToCameraRotationMatrix = MathLab::GetRotationFromTransformMatrix(worldToCameraTransform);

        //Lighting
        for (int i = 0; i < m_lightList.Count(); i++)
        {
            assert(m_lightList[i]->GetLightType() != LightComponent::LightType::None);
            if (m_lightList[i]->GetLightType() == LightComponent::LightType::Direction)
            {
                DirectionLightComponent* directionLight = static_cast<DirectionLightComponent*>(m_lightList[i]);
                Vector3f direction = directionLight->GetDirection();
                direction = -direction;
                TVector4f tDirection = ConvertFromNonHomogeneous(direction);
                tDirection = tDirection * worldToCameraRotationMatrix;
                for (int faceIndex = 0; faceIndex < 3; faceIndex++)
                {
                    TVector4f faceNormal = input.m_triangle[faceIndex].m_normal;
                    float face = tDirection.Dot(faceNormal);
                    if (face > 0)
                    {
                        CrossPlatform::SColorRGBA baseColor = input.m_baseColor[faceIndex];
                        output.m_vertexColor[faceIndex] += directionLight->GetColor() * baseColor * face;
                    }
                }
            }
            if (m_lightList[i]->GetLightType() == LightComponent::LightType::Point)
            {
                PointLightComponent* pointLight = static_cast<PointLightComponent*>(m_lightList[i]);
                Vector3f lightPosition = pointLight->GetGameObject()->GetComponentPointer<Transform3D>()->GetPosition();
                TVector4f lightPoistionHomogeneous = MathLab::ConvertFromNonHomogeneous(lightPosition);
                lightPoistionHomogeneous = lightPoistionHomogeneous * worldToCameraTransform;
                for (int faceIndex = 0; faceIndex < 3; faceIndex++)
                {
                    TVector4f direction = lightPoistionHomogeneous - input.m_triangle[faceIndex].m_vertex;
                    float kc = pointLight->GetKc();
                    float kl = pointLight->GetKl();
                    float face = direction.Normalize().Dot(input.m_triangle[faceIndex].m_normal);
                    if (face > 0)
                    {
                        float attenuation = kc + kl * direction.Magnitude();
                        CrossPlatform::SColorRGBA baseColor = input.m_baseColor[faceIndex];
                        output.m_vertexColor[faceIndex] += pointLight->GetColor() * baseColor *  face / attenuation;
                    }
                }

            }
        }
        for (int i = 0; i < 3; i++)
        {
            Vector3f screenPosition1 = ConvertFormHomogeneous(input.m_triangle[i].m_vertex * cameraToScreenTranform);
            output.m_screenPoint[i] = Vector2f(screenPosition1.x, screenPosition1.y);
        }
        return true;
    }

	void Graphic3D::Update() const
	{
        const Camera3D* majorCamera = GetMajorCamera();
        if (!majorCamera)
        {
            return;
        }
		const GameObjectWorld* world = EngineManager::Get().GetWorldManager().GetCurrentWorld();
		if (!world)
		{
			return;
		}
		TMatrix4X4f worldToCameraTransform = majorCamera->GetCameraTransformInverseMatrix();
		TMatrix4X4f worldToCameraRotationMatrix = MathLab::GetRotationFromTransformMatrix(worldToCameraTransform);

		TMatrix4X4f cameraToScreenTranform = GetProjectionAndScreenMatrix(majorCamera->GetFov(), majorCamera->GetViewport());
		TMatrix4X4f worldToScreenTranform = worldToCameraTransform * cameraToScreenTranform;
		DrawCoordinateLines(worldToScreenTranform);

		TMatrix4X4f cameraToProjectionTransfrom = GetProjectionMatrix(majorCamera->GetViewDistance(), majorCamera->GetAspectRatio());
		TMatrix4X4f projectionToScreenTransfrom = GetScreenMatrix(majorCamera->GetViewport());

		const Algorithm::Vector<GameObject*>& renderList = world->GetRenderList();
		for (int i = 0; i < renderList.Count(); i++)
		{
			GameObject* gameobject = renderList[i];
			Transform3D* transform = gameobject->GetComponentPointer<Transform3D>();
			Mesh3D* mesh = gameobject->GetComponentPointer<Mesh3D>();
			if (!mesh || !transform || !mesh->IsValid())
			{
				continue;
			}

			// World coordinate, Culling
			TMatrix4X4f localToWorldTransform = transform->GetLocalToWorldTransformMatrix();
			TMatrix4X4f localToScreenTranform = localToWorldTransform * worldToScreenTranform;
			TMatrix4X4f localToCameraTranform = localToWorldTransform * worldToCameraTransform;

			CullingState state = Culling(*mesh, localToCameraTranform, *majorCamera);
			if (state == CullingState::Culled)
			{
				continue;
			}

			//// Inserting rendering list
			//const Vector<Material*>& materials = mesh->GetMaterials();
			//m_renderList.AddMesh3D();

			// Per Triangle Stage
			for (auto iter = mesh->begin(); iter != mesh->end(); iter++)
			{
				Triangle3D triangle(*iter);
				//triangle[0] = *iter;
				//triangle[1] = *(iter + 1);
				//triangle[2] = *(iter + 2);


				// [Vertex] Transform Local into Camera
				// [Normal] Transform World into Camera
				TransformLocalToCamera(triangle, localToCameraTranform, worldToCameraRotationMatrix);

				// Remove Back Faces
				if (m_isRemoveBackFacesDebug)
				{
					CullingState removeBackFacesState = RemoveBackFaces(triangle);
					if (removeBackFacesState == CullingState::Culled)
					{
						continue;
					}
				}

				// Clipping near Z-axis triangle
				ClipResult clipResult = Clip(triangle, *majorCamera);
				if (clipResult.m_clippingState == ClippingState::NeedToClipWithAdditionalVertex)
				{
					//TODO
					// Render additional triangle
				}

				// Get shader
				Material& material = mesh->GetMaterial(triangle.m_materialIndex);
				ShaderType shaderType = material.GetShaderType();
				if (shaderType == ShaderType::ShaderType_Wireframe || shaderType == ShaderType::ShaderType_Flat)
				{
					VertexShaderDataInputFlat input;
					input.m_triangle = triangle;
					input.m_faceColor = CrossPlatform::WHITE;
					VertexShaderDataOutputFlat output;
					VertexShaderFlat(input, output, worldToCameraTransform, cameraToScreenTranform);
					if (shaderType == ShaderType::ShaderType_Flat)
					{
						Graphic2D::Get().DrawTriangle(output.m_screenPoint0, output.m_screenPoint1, output.m_screenPoint2, output.m_faceColor);
					}
					if (shaderType == ShaderType::ShaderType_Wireframe)
					{
						Vector2f tmp0;
						Vector2f tmp1;
						tmp0 = output.m_screenPoint0;
						tmp1 = output.m_screenPoint1;
						Graphic2D::ClipLineState lineClipState = Graphic2D::Get().ClipLine(tmp0, tmp1);
						if (lineClipState == Graphic2D::ClipLineState::Accpet)
						{
							Graphic2D::Get().DrawLine(tmp0, tmp1);
						}
						tmp0 = output.m_screenPoint1;
						tmp1 = output.m_screenPoint2;
						lineClipState = Graphic2D::Get().ClipLine(tmp0, tmp1);
						if (lineClipState == Graphic2D::ClipLineState::Accpet)
						{
							Graphic2D::Get().DrawLine(tmp0, tmp1);
						}
						tmp0 = output.m_screenPoint2;
						tmp1 = output.m_screenPoint0;
						lineClipState = Graphic2D::Get().ClipLine(tmp0, tmp1);
						if (lineClipState == Graphic2D::ClipLineState::Accpet)
						{
							Graphic2D::Get().DrawLine(tmp0, tmp1);
						}
					}
				}

				if (shaderType == ShaderType::ShaderType_Gouraud)
				{
					//const Triangle& normal = normalList[sortingTriangleIndexList[polyIndex].m_index];
					if (triangle[0].m_normal == TVector4f::Zero || 
						triangle[1].m_normal == TVector4f::Zero || 
						triangle[2].m_normal == TVector4f::Zero)
					{
						printf("Gouraud Shadering must contain normal");
						break;
					}
					VertexShaderDataInputGouraud input;
					input.m_triangle = triangle;
					//input.m_normal = normal;
					input.m_baseColor[0] = CrossPlatform::WHITE;
					input.m_baseColor[1] = CrossPlatform::WHITE;
					input.m_baseColor[2] = CrossPlatform::WHITE;
					VertexShaderDataOutputGouraud output;
					VertexShaderGouraud(input, output, worldToCameraTransform, cameraToScreenTranform);
					VertexData vertexData;
					vertexData.p0 = output.m_screenPoint[0];
					vertexData.p1 = output.m_screenPoint[1];
					vertexData.p2 = output.m_screenPoint[2];
					vertexData.vcolor0 = output.m_vertexColor[0];
					vertexData.vcolor1 = output.m_vertexColor[1];
					vertexData.vcolor2 = output.m_vertexColor[2];

					if (triangle.m_materialIndex != -1)
					{
						VertexWithMaterialData data;
						data.m_data = vertexData;
						data.m_diffuse = material.GetDiffuseTexture();
						data.uv0 = triangle[0].m_uv;
						data.uv1 = triangle[1].m_uv;
						data.uv2 = triangle[2].m_uv;
						Graphic2D::Get().DrawTriangle(data);

					}
					else
					{
						Graphic2D::Get().DrawTriangle(vertexData);
					}
				}
			}
		}
	}

/*
			for (int polygonIndex = 0; polygonIndex < polygons.Count(); polygonIndex++)
			{
				Polygon3D renderingPolygon(*(polygons[polygonIndex]));


				int triangleCount = polygon->Count() / 3;
				Triangle* triangleList = new Triangle[triangleCount];
				Triangle* normalList = new Triangle[triangleCount];
				TriangleIndex* sortingTriangleIndexList = new TriangleIndex[triangleCount];
				for (int polyIndex = 0; polyIndex < triangleCount; polyIndex++)
				{
					Vertex3D node0 = (*polygon)[polyIndex * 3];
					Vertex3D node1 = (*polygon)[polyIndex * 3 + 1];
					Vertex3D node2 = (*polygon)[polyIndex * 3 + 2];

					const Vector3f& vertex0 = node0.m_vertex;
					const Vector3f& vertex1 = node1.m_vertex;
					const Vector3f& vertex2 = node2.m_vertex;
					TVector4f homogeneousVertex0 = ConvertFromNonHomogeneous(vertex0);
					TVector4f homogeneousVertex1 = ConvertFromNonHomogeneous(vertex1);
					TVector4f homogeneousVertex2 = ConvertFromNonHomogeneous(vertex2);
					homogeneousVertex0 = homogeneousVertex0 * localToCameraTranform;
					homogeneousVertex1 = homogeneousVertex1 * localToCameraTranform;
					homogeneousVertex2 = homogeneousVertex2 * localToCameraTranform;
					triangleList[polyIndex][0] = homogeneousVertex0;
					triangleList[polyIndex][1] = homogeneousVertex1;
					triangleList[polyIndex][2] = homogeneousVertex2;

					sortingTriangleIndexList[polyIndex].m_index = polyIndex;
					sortingTriangleIndexList[polyIndex].m_zAixs = (homogeneousVertex0[2] + homogeneousVertex1[2] + homogeneousVertex2[2]) / 3;

					const Vector3f& normal0 = node0.m_normal;
					const Vector3f& normal1 = node1.m_normal;
					const Vector3f& normal2 = node2.m_normal;
					if (normal0 == Vector3f::Zero || normal1 == Vector3f::Zero || normal2 == Vector3f::Zero)
					{
						normalList[polyIndex][0] = TVector4f::Zero;
						normalList[polyIndex][1] = TVector4f::Zero;
						normalList[polyIndex][2] = TVector4f::Zero;
						continue;
					}
					TVector4f homogeneousNormal0 = ConvertFromNonHomogeneous(normal0);
					TVector4f homogeneousNormal1 = ConvertFromNonHomogeneous(normal1);
					TVector4f homogeneousNormal2 = ConvertFromNonHomogeneous(normal2);
					homogeneousNormal0 = homogeneousNormal0 * worldToCameraRotationMatrix;
					homogeneousNormal1 = homogeneousNormal1 * worldToCameraRotationMatrix;
					homogeneousNormal2 = homogeneousNormal2 * worldToCameraRotationMatrix;
					normalList[polyIndex][0] = homogeneousNormal0.Normalize();
					normalList[polyIndex][1] = homogeneousNormal1.Normalize();
					normalList[polyIndex][2] = homogeneousNormal2.Normalize();
				}
				int triangleCount = renderingPolygon.Count() / 3;
				TriangleIndex* sortingTriangleIndexList = new TriangleIndex[triangleCount];
				Algorithm::Sort<TriangleIndex> sort;
				sort.Quick(sortingTriangleIndexList, triangleCount, IsIndexZAxisBigger);

				//Shadering and Camera to Screen (Projection) Transform

				for (int polyIndex = 0; polyIndex < triangleCount; polyIndex++)
				{
					//const Triangle& triangle = triangleList[polyIndex];
					const Triangle& triangle = triangleList[sortingTriangleIndexList[polyIndex].m_index];

					CullingState removeBackFacesState = RemoveBackFaces(triangle[0], triangle[1], triangle[2]);
					if (removeBackFacesState == Culling0State::Culled)
					{
						continue;
					}



				delete[] triangleList;
				delete[] normalList;
				delete[] sortingTriangleIndexList;
			}
		}
*/

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
        return CullingState::NotCulled;
    }

	Graphic3D::ClipResult Graphic3D::Clip(Triangle3D& triangle, const Camera3D& camera) const
	{
		float distance = camera.GetViewDistance();
		PlaneTestState state[3];

		ClipResult clipResult;
		clipResult.m_clippingState = ClippingState::Inside;

		for (int i = 0; i < 3; i++)
		{
			Vector3f center = ConvertFormHomogeneous(triangle[i].m_vertex);
			float xLimit = 0.5f * camera.GetViewPlane().x * center.z / distance;
			if (center.x > xLimit)
			{
				state[i] = PlaneTestState::GreaterThanXMax;
			}
			else if (center.x < -xLimit)
			{
				state[i] = PlaneTestState::LessThanXMin;
			}
			else
			{
				state[i] = PlaneTestState::InsideX;
			}
		}
		if ((state[0] == PlaneTestState::GreaterThanXMax && state[1] == PlaneTestState::GreaterThanXMax && state[2] == PlaneTestState::GreaterThanXMax) ||
			(state[0] == PlaneTestState::LessThanXMin && state[1] == PlaneTestState::LessThanXMin && state[2] == PlaneTestState::LessThanXMin))
		{
			clipResult.m_clippingState = ClippingState::Clipped;
		}

		for (int i = 0; i < 3; i++)
		{
			Vector3f center = ConvertFormHomogeneous(triangle[i].m_vertex);
			float yLimit = 0.5f * camera.GetViewPlane().y * center.z / distance;
			if (center.y > yLimit)
			{
				state[i] = PlaneTestState::GreaterThanYMax;
			}
			else if (center.y < -yLimit)
			{
				state[i] = PlaneTestState::LessThanYMin;
			}
			else
			{
				state[i] = PlaneTestState::InsideY;
			}
		}
		if ((state[0] == PlaneTestState::GreaterThanYMax && state[1] == PlaneTestState::GreaterThanYMax && state[2] == PlaneTestState::GreaterThanYMax) ||
			(state[0] == PlaneTestState::LessThanYMin && state[1] == PlaneTestState::LessThanYMin && state[2] == PlaneTestState::LessThanYMin))
		{
			clipResult.m_clippingState = ClippingState::Clipped;
		}

		Vector<Vertex3D> outOfNearZ;
		for (int i = 0; i < 3; i++)
		{
			Vector3f center = ConvertFormHomogeneous(triangle[i].m_vertex);
			if (center.z < camera.GetNearClipZ())
			{
				state[i] = PlaneTestState::LessThanZMin;
				outOfNearZ.Add(triangle[i]);
			}
			else if (center.z > camera.GetFarClipZ())
			{
				state[i] = PlaneTestState::GreaterThanZMax;
			}
			else
			{
				state[i] = PlaneTestState::InsideZ;
			}
		}
		if ((state[0] == PlaneTestState::LessThanZMin && state[1] == PlaneTestState::LessThanZMin && state[2] == PlaneTestState::LessThanZMin) ||
			(state[0] == PlaneTestState::GreaterThanZMax && state[1] == PlaneTestState::GreaterThanZMax && state[2] == PlaneTestState::GreaterThanZMax))
		{
			clipResult.m_clippingState = ClippingState::Clipped;
		}
		else if (state[0] == PlaneTestState::LessThanZMin ||
			state[1] == PlaneTestState::LessThanZMin ||
			state[2] == PlaneTestState::LessThanZMin)
		{
			float nearZ = camera.GetNearClipZ();

			if (outOfNearZ.Count() == 1)
			{
				clipResult.m_clippingState = ClippingState::NeedToClipWithAdditionalVertex;

				Vertex3D outNearZp0;
				Vertex3D insideZp1;
				Vertex3D insideZp2;
				if (state[0] == PlaneTestState::LessThanZMin)
				{
					outNearZp0 = triangle[0];
					insideZp1 = triangle[1];
					insideZp2 = triangle[2];
				}
				else if (state[1] == PlaneTestState::LessThanZMin)
				{
					outNearZp0 = triangle[1];
					insideZp1 = triangle[2];
					insideZp2 = triangle[0];
				}
				else if (state[2] == PlaneTestState::LessThanZMin)
				{
					outNearZp0 = triangle[2];
					insideZp1 = triangle[0];
					insideZp2 = triangle[1];
				}
				else
				{
					assert(true == false);
				}
				outNearZp0 = InternalClipZPoint(outNearZp0, insideZp1, nearZ);

				triangle[0] = outNearZp0;
				triangle[1] = insideZp1;
				triangle[2] = insideZp2;
				clipResult.m_additionalGeneratedTriangle[0] = InternalClipZPoint(outNearZp0, insideZp2, nearZ);
				clipResult.m_additionalGeneratedTriangle[1] = outNearZp0;
				clipResult.m_additionalGeneratedTriangle[2] = insideZp2;
			}
			else if (outOfNearZ.Count() == 2)
			{
				clipResult.m_clippingState = ClippingState::NeedToClip;

				Vertex3D insideZp0;
				Vertex3D outNearZp1;
				Vertex3D outNearZp2;
				if (state[0] == PlaneTestState::InsideZ)
				{
					insideZp0 = triangle[0];
					outNearZp1 = triangle[1];
					outNearZp2 = triangle[2];
				}
				else if (state[1] == PlaneTestState::InsideZ)
				{
					insideZp0 = triangle[1];
					outNearZp1 = triangle[2];
					outNearZp2 = triangle[0];
				}
				else if (state[2] == PlaneTestState::InsideZ)
				{
					insideZp0 = triangle[2];
					outNearZp1 = triangle[0];
					outNearZp2 = triangle[1];
				}
				else
				{
					assert(true == false);
				}
				triangle[1] = InternalClipZPoint(outNearZp1, insideZp0, nearZ);
				triangle[2] = InternalClipZPoint(outNearZp2, insideZp0, nearZ);
			}
			else
			{
				assert(true == false); //out of nearZ vertex number must >0 and <3
			}
		}

		return clipResult;
	}

	//Graphic3D::CullingState Graphic3D::RemoveBackFaces(const TVector4f& p0, const TVector4f& p1, const TVector4f& p2) const
 //   {
 //       TVector4f u = p1 - p0;
 //       TVector4f v = p2 - p1;
 //       TVector4f n = u.Cross(v);

 //       if( p0.Dot(n) <= 0)
 //       {
 //           return CullingState::NotCulled;
 //       }
 //       else
 //       {
 //           return CullingState::Culled;
 //       }
 //   }

	//XenonEngine::Graphic3D::CullingState Graphic3D::RemoveBackFaces(const Vertex3D& p0, const Vertex3D& p1, const Vertex3D& p2) const
	//{
	//	TVector4f u = p1.m_vertex - p0.m_vertex;
	//	TVector4f v = p2.m_vertex - p1.m_vertex;
	//	TVector4f n = u.Cross(v);

	//	if (p0.Dot(n) <= 0)
	//	{
	//		return CullingState::NotCulled;
	//	}
	//	else
	//	{
	//		return CullingState::Culled;
	//	}
	//}

	XenonEngine::Graphic3D::CullingState Graphic3D::RemoveBackFaces(const Triangle3D& triangle) const
	{
		TVector4f u = triangle[1].m_vertex - triangle[0].m_vertex;
		TVector4f v = triangle[2].m_vertex - triangle[1].m_vertex;
		TVector4f n = u.Cross(v);

		if (triangle[0].m_vertex.Dot(n) <= 0)
		{
			return CullingState::NotCulled;
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
        //assert(m_cameraList.Count() != 0);
        if (m_cameraList.Count() == 0)
        {
            return nullptr;
        }
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

    MathLab::TMatrix4X4f Graphic3D::GetScreenMatrix(const MathLab::Vector2i& viewPort) const
    {
        float alpha = 0.5f * viewPort.x - 0.5f;
        float beta = 0.5f * viewPort.y - 0.5f;
        return TMatrix4X4f(
            std::initializer_list<float>{
                alpha, 0, 0, 0,
                0, beta, 0, 0,
                alpha, beta, 1, 0,
                0, 0, 0, 1
        });
    }

	MathLab::TMatrix4X4f Graphic3D::GetProjectionAndScreenMatrix(const float fov, const MathLab::Vector2i& viewPort) const
    {
        float viewDistance = tan(fov/2) * (viewPort.x / 2);
        float alpha = 0.5f * viewPort.x - 0.5f;
        float beta = 0.5f * viewPort.y - 0.5f;
        return TMatrix4X4f(
            std::initializer_list<float>{
                viewDistance, 0, 0, 0,
                0, viewDistance, 0, 0,
                alpha, beta, 1, 1,
                0, 0, 0, 0
        });
    }

    //bool IsZAxisBigger(const Triangle& lhs, const Triangle& rhs)
    //{
    //    return (lhs[0][2] + lhs[1][2] + lhs[2][2]) /3 > (rhs[0][2] + rhs[1][2] + rhs[2][2]) / 3;
    //}

    //bool IsIndexZAxisBigger(const TriangleIndex& lIndex, const TriangleIndex& rIndex)
    //{
    //    return lIndex.m_zAixs > rIndex.m_zAixs;
    //}

	void Graphic3D::TransformLocalToCamera(Triangle3D& triangle , const MathLab::TMatrix4X4f& localToCameraTranform, const MathLab::TMatrix4X4f& worldToCameraRotationMatrix)const
	{
		triangle[0].m_vertex = triangle[0].m_vertex * localToCameraTranform;
		triangle[0].m_normal = triangle[0].m_normal * worldToCameraRotationMatrix;
		triangle[1].m_vertex = triangle[1].m_vertex * localToCameraTranform;
		triangle[1].m_normal = triangle[1].m_normal * worldToCameraRotationMatrix;
		triangle[2].m_vertex = triangle[2].m_vertex * localToCameraTranform;
		triangle[2].m_normal = triangle[2].m_normal * worldToCameraRotationMatrix;
	}

	//TVector4f Graphic3D::InternalClipZPoint(const TVector4f& outsidePoint, const TVector4f& insidePoint, int clipZ) const
	//{
	//	TVector4f newPoint(outsidePoint);
	//	float radio = (clipZ - insidePoint[2]) / (outsidePoint[2] - insidePoint[2]);
	//	newPoint[0] = radio * (outsidePoint[0] - insidePoint[0]) + insidePoint[0];
	//	newPoint[1] = radio * (outsidePoint[1] - insidePoint[1]) + insidePoint[1];
	//	newPoint[2] = clipZ;
	//	return newPoint;
	//}

	CrossPlatform::Vertex3D Graphic3D::InternalClipZPoint(const CrossPlatform::Vertex3D& outsideVertex, const CrossPlatform::Vertex3D& insideVertex, float clipZ) const
	{
		Vertex3D newVertex(outsideVertex);
		float radio = (clipZ - insideVertex.m_vertex[2]) / (outsideVertex.m_vertex[2] - insideVertex.m_vertex[2]);
		newVertex.m_vertex[0] = radio * (outsideVertex.m_vertex[0] - insideVertex.m_vertex[0]) + insideVertex.m_vertex[0];
		newVertex.m_vertex[1] = radio * (outsideVertex.m_vertex[1] - insideVertex.m_vertex[1]) + insideVertex.m_vertex[1];
		newVertex.m_vertex[2] = clipZ;
		newVertex.m_normal[0] = radio * (outsideVertex.m_normal[0] - insideVertex.m_normal[0]) + insideVertex.m_normal[0];
		newVertex.m_normal[1] = radio * (outsideVertex.m_normal[1] - insideVertex.m_normal[1]) + insideVertex.m_normal[1];
		newVertex.m_normal[2] = radio * (outsideVertex.m_normal[2] - insideVertex.m_normal[2]) + insideVertex.m_normal[2];
		return newVertex;
	}

}