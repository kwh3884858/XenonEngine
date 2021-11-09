#include "EditorGameObject.h"
#include "Library/imgui/imgui.h"
#include "Engine/GameObject.h"

#include "Engine/Component/Transform3D.h"
#include "Engine/Component/DirectionLightComponent.h"
#include "Engine/Component/PointLightComponent.h"


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
                            DirectionLightComponent* directionLight =  (DirectionLightComponent*)light;
                            const Vector3f& pos = directionLight->GetDirection();
                            DragFloat3("Direction", const_cast<Vector3f&>(pos), 0.01f, -100.0f, 100.0f);
                            const Vector4f& color = directionLight->GetRawColor();
                            ColorEdit4("LightColor", const_cast<Vector4f&>(color));
                            ImGui::TreePop();
                        }
                    }
                }
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

}