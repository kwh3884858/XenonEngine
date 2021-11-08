#include "EditorGameObjectWorld.h"
#include "Engine/EngineSyncData.h"

#include "Engine/GameObjectWorld.h"
#include "Engine/GameObject.h"

#include "Library/imgui/imgui.h"

#include "Engine/Editor/EditorGameObject.h"
#include "Engine/Graphic/Graphic3D.h"
namespace XenonEngine
{

    //void EditorGUI::Initialize(const EngineSyncData* data)
    //{
    //    m_worldData = data->m_gameWorld;
    //    
    //}

    //void EditorGUI::Shutdown()
    //{

    //}

    //void EditorGameObjectWorld::Update(const EngineSyncData* data)
    //{

    //}

    void EditorGameObjectWorld::UpdateMainWindow(const void* data /*= nullptr*/)
    {
        const EngineSyncData* syncData = static_cast<const EngineSyncData*>(data);
        const Graphic3D* graphic = syncData->Graphic3DGetter();
        if (!graphic)
        {
            return;
        }
        const Graphic3D::RenderType& renderType = graphic->GetRenderType();
        const char* items[] = { "Wireframe", "FlatShdering", "GouraudShdering"};
        const char* item_current = items[renderType];            // Here our selection is a single pointer stored outside the object.
        if (ImGui::BeginCombo("combo 1", item_current)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (item_current == items[n]);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    item_current = items[n];
                    ((Graphic3D*)graphic)->SetRenderType((Graphic3D::RenderType)n);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }

        const GameObjectWorld* world = syncData->WorldGetter();
        if (!world)
        {
            return;
        }
        EditorGameObject editorGameobject;
        if (ImGui::TreeNode("Scene"))
        {
            const Algorithm::Vector<GameObject*>& objectList = world->GetWorldObjects();
            for (int i = 0; i < objectList.Count(); i++)
            {
                const GameObject* go =objectList[i];
                if (ImGui::TreeNode((void*)(intptr_t)i, go->GetName().CString()))
                {
                    editorGameobject.Update(go);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }

}