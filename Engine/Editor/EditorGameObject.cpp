#include "EditorGameObject.h"
#include "Library/imgui/imgui.h"
#include "Engine/GameObject.h"

#include "Engine/Component/Transform3D.h"
#include "Engine/Component/DirectionLightComponent.h"
#include "Engine/Component/PointLightComponent.h"
#include "Engine/Component/Mesh3D.h"
#include "Engine/Component/Camera3D.h"
#include "CrossPlatform/Material/Material.h"


namespace XenonEngine
{
    using namespace MathLab;
    using namespace CrossPlatform;

    void EditorGameObject::Update(const GameObject* data /*= nullptr*/)
    {
        for (int i = 0; i < data->GetComponentCount(); i++)
        {
            const IComponent* component = data->GetComponentByIndex(i);
            ComponentType type = component->GetComponentType();
            if (type == ComponentType::ComponentType_Transform3D)
            {
                if (ImGui::TreeNode((void*)(intptr_t)i, "%d Transform3D", i))
                {
					const Transform3D* trans3D = static_cast<const Transform3D*>(component);
                    const Vector3f& pos = trans3D->GetPosition();
                    DragFloat3("Transform", const_cast<Vector3f&>(pos), 0.01f, -100.0f, 100.0f);
                    const Vector3f& rot = trans3D->GetRotation();
                    DragFloat3("Rotation", const_cast<Vector3f&>(rot), 0.01f, -100.0f, 100.0f);
                    ImGui::TreePop();
                }
            }
            if (type == ComponentType::ComponentType_Light)
            {
				const LightComponent* light = static_cast<const LightComponent*>(component);
                LightComponent::LightType lightType = light->GetLightType();
                if (lightType == LightComponent::LightType::Direction)
                {
                    if (ImGui::TreeNode((void*)(intptr_t)i, "%d Direction Light Component", i))
                    {
						ContextMenu((GameObject*)data, (IComponent*)component);
						DirectionLightComponent* directionLight = (DirectionLightComponent*)light;
                        const Vector3f& pos = directionLight->GetDirection();
                        DragFloat3("Direction", const_cast<Vector3f&>(pos), 0.01f, -100.0f, 100.0f);
                        const Vector4f& color = directionLight->GetRawColor();
                        ColorEdit4("LightColor", const_cast<Vector4f&>(color));
                        ImGui::TreePop();
                    }
                }
            }
            if (type == ComponentType::ComponentType_Mesh3D)
            {
                if (ImGui::TreeNode((void*)(intptr_t)i, "%d Mesh3D", i))
                {
					ContextMenu((GameObject*)data, (IComponent*)component);
					const Mesh3D* mesh = static_cast<const Mesh3D*>(component);
                    ImGui::PushID(i);
                    ImGui::Text("Model GUID: %s", mesh->GetModelGuid().str().c_str());
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE_GUID"))
                        {
                            IM_ASSERT(payload->DataSize == 37);
                            char* payload_n = (char*)payload->Data;
                            ( (Mesh3D*)mesh )->SetModelGuid(xg::Guid(payload_n));
							( (Mesh3D*)mesh )->LoadModel();
                        }
                        ImGui::EndDragDropTarget();
                    }
                    ImGui::PopID();
					const Vector<Material*> materials = mesh->GetMaterials();
					for (int i = 0 ; i < materials.Count(); i++)
					{
						const Material* material = materials[i];
						ImGui::Text("Material Name: %s", material->GetName().CString());
						ImGui::Text("Exponent: %f", material->GetExponent());
						ImGui::Text("Ambient: "); ImGui::SameLine(); Text(material->GetAmbient());
						ImGui::Text("Diffuse: "); ImGui::SameLine(); Text(material->GetDiffuse());
						ImGui::Text("Specular: "); ImGui::SameLine(); Text(material->GetSpecular());
						ImGui::Text("Emission: "); ImGui::SameLine(); Text(material->GetEmission());
						ImGui::Text("Material Texture Name: %s", material->GetDiffuseTexture().CString());
						ImGui::Text("Bump Texture Name: %s", material->GetBumpTexture().CString());
					}
                    ImGui::Text("Max Radius: %f", mesh->GetMaxRadius());
                    ImGui::TreePop();
                }
            }
            if (type == ComponentType::ComponentType_Camera)
            {
                if (ImGui::TreeNode((void*)(intptr_t)i, "%d Camera", i))
                {
					ContextMenu((GameObject*)data, (IComponent*)component);
                    const Camera3D* mesh = static_cast<const Camera3D*>(component);
                    const Vector3f lookat = mesh->GetLookAt();
                    ImGui::Text("Look At:"); ImGui::SameLine(); Text(lookat);
                    ImGui::Text("Fov: %f", mesh->GetFov());
                    ImGui::Text("View Distance: %f", mesh->GetViewDistance());
                    ImGui::Text("View Plane: "); ImGui::SameLine(); Text(mesh->GetViewPlane());
                    ImGui::Text("View Port: "); ImGui::SameLine(); Text(mesh->GetViewport());
                    ImGui::TreePop();
                }
            }
        }
    }

	void EditorGameObject::ContextMenu(GameObject* gameobject, IComponent* component) const
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove Component")) {
				component->SetState(IComponent::ObjectState::MarkForDelete);
			}
			ImGui::EndPopup();
		}
	}

	bool EditorGameObject::DragFloat3(const char* label, MathLab::Vector3f& v, float v_speed /*= 1.0f*/, float v_min /*= 0.0f*/, float v_max /*= 0.0f*/, const char* format /*= "%.3f"*/, float power /*= 1.0f*/)
    {
        return ImGui::DragScalarN(label, ImGuiDataType_Float, &v, 3, v_speed, &v_min, &v_max, format, power);
    }

    bool EditorGameObject::ColorEdit4(const char* label, MathLab::Vector4f& col)
    {
        float* color = reinterpret_cast<float*>(&col);
        return ImGui::ColorEdit4(label, color);
    }
    void EditorGameObject::Text(const MathLab::Vector2f& value)
    {
        ImGui::Text("%f, %f", value.x, value.y);
    }

    void EditorGameObject::Text(const MathLab::Vector3f& value)
    {
        ImGui::Text("%f, %f, %f", value.x, value.y, value.z);
    }
}