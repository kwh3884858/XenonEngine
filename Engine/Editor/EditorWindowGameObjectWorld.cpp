#include "EditorWindowGameObjectWorld.h"

#include "Engine/EngineSyncData.h"
#include "Engine/FileDatabase.h"
#include "Engine/GameObjectWorldManager.h"
#include "Engine/GameObjectWorld.h"
#include "Engine/GameObject.h"
#include "Engine/Editor/EditorGameObject.h"
#include "Engine/Graphic/Graphic3D.h"

#include "Library/imgui/imgui.h"
#include "Library/IconFontCppHeaders/IconsFontAwesome5.h"

#include "CrossPlatform/File/FolderMeta.h"
#include "CrossPlatform/Editor/XenonEditorInterface.h"
#include <filesystem>
#include "EditorDatabase.h"
#include "Engine/Component/PointLightComponent.h"
#include "Engine/Component/DirectionLightComponent.h"
#include "Engine/Component/Camera3D.h"
#include "CrossPlatform/Database.h"
//#include "CrossPlatform/FileTypeEnum.h"

namespace XenonEngine
{
    using namespace CrossPlatform;
    using namespace std;
    using namespace std::filesystem;
    using namespace MathLab;
    void EditorWindowGameObjectWorld::UpdateMainWindow(const void* data /*= nullptr*/)
    {
        const EngineSyncData* syncData = static_cast<const EngineSyncData*>(data);
        const Graphic3D* graphic = syncData->Graphic3DGetter();
        if (!graphic)
        {
            return;
        }

        const Graphic3D::RenderType& renderType = graphic->GetRenderType();
        const char* items[] = { "Wireframe", "FlatShdering", "GouraudShdering" };
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

        const GameObjectWorldManager* worldManager = syncData->WorldManagerGetter();
        if (!worldManager)
        {
            return;
        }
        GameObjectWorld*const world = worldManager->GetCurrentWorld();
        Algorithm:String worldName = world->GetWorldName();

        // World Name

        ImGui::Text("World Name:"); ImGui::SameLine();
        ImGui::InputText("##InputworldName", worldName);
        if (worldName != world->GetWorldName())
        {
            world->SetWorldName(worldName);
        }

        FileDatabase* database = (FileDatabase*)syncData->DatabaseGetter();
        if (!database)
        {
            return;
        }
        EditorDatabase::Get().SetFileDatabase(database);

        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("New World")) {

                }
                if (ImGui::MenuItem("Open World", "Ctrl+O"))
                {
                    const char *filters = "World (*.world){.world}";
                    m_loadDialog.OpenDialog("LoadWorld", ICON_FA_FILE "Load World", filters, database->GetRootFolder()->GetFileHeader().GetVirtualPath().CString(), true);
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    database->SaveFile(CrossPlatform::FileHeader::Root_Drive + std::filesystem::path::preferred_separator + worldName + database->GetExtension(FileType::FileTypeWorld));
                }
                if (ImGui::MenuItem("Save as World", "Ctrl+Shift+S")) {
                    m_saveDialog.OpenDialog("ChooseFolder", ICON_FA_AMBULANCE "Choose Folder to Save World", nullptr, database->GetRootFolder()->GetFileHeader().GetVirtualPath().CString(), true);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImVec2 maxSize = ImVec2((float)1280, (float)720);
        ImVec2 minSize = ImVec2((float)640, (float)360);
        if (m_loadDialog.Display("LoadWorld", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
        {
            if (m_loadDialog.IsOk())
            {
                database->LoadFile(m_loadDialog.GetFilePathName().c_str());
            }
            m_loadDialog.Close();
        }
        if (m_saveDialog.Display("ChooseFolder", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
        {
            if (m_saveDialog.IsOk())
            {
                path filePathName = m_saveDialog.GetFilePathName();
                filePathName.append((worldName + database->GetExtension(FileType::FileTypeWorld)).CString());
                database->SaveFile(filePathName.string().c_str());
            }
            m_saveDialog.Close();
        }

        // Main Window
        EditorGameObject editorGameobject;
        if (ImGui::TreeNode("Scene"))
        {
            ContextMenu(world);
            const Algorithm::Vector<GameObject*>& objectList = world->GetWorldObjects();
            for (int i = 0; i < objectList.Count(); i++)
            {
                const GameObject* go = objectList[i];
                if (ImGui::TreeNode((void*)(intptr_t)i, go->GetName().CString()))
                {
                    ContextMenu(world, (GameObject*)go);
                    editorGameobject.Update(go);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }

    void EditorWindowGameObjectWorld::ContextMenu(GameObjectWorld* world, GameObject* go /*= nullptr*/) const
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Add GameObject")) {
                GameObject* newGo =new GameObject();
                Transform3D* trans3d = new Transform3D(newGo);
                newGo->AddComponent(trans3d);
                world->AddGameObject(newGo);
            }
            if (go)
            {
                if (ImGui::BeginMenu("Add Component"))
                {
                    if (ImGui::MenuItem("Transform 3D")) {
                        Transform3D* trans3D = new Transform3D(go);
                        trans3D->Start();
                        go->AddComponent(trans3D);
                    }
                    if (ImGui::MenuItem("Mesh 3D")) {
                        Mesh3D* mesh3D = new Mesh3D(go);
                        mesh3D->Start();
                        go->AddComponent(mesh3D);
                    }
                    if (ImGui::MenuItem("Direction Light")) {
                        DirectionLightComponent* directLight = new DirectionLightComponent(go);
                        directLight->Start();
                        go->AddComponent(directLight);
                    }
                    if (ImGui::MenuItem("Point Light")) {
                        PointLightComponent* pointLight = new PointLightComponent(go);
                        pointLight->Start();
                        go->AddComponent(pointLight);
                    }
                    if (ImGui::MenuItem("Camera")) {
                        Camera3DConfig camera3DConfig;
                        camera3DConfig.m_fov = 90;
                        camera3DConfig.m_viewport = Vector2f(Database::Get().engineConfig.m_width, Database::Get().engineConfig.m_height);
                        camera3DConfig.m_farClipZ = 1000;
                        camera3DConfig.m_nearClipZ = 1;
                        Camera3D* camera = new Camera3D(go);
                        camera->SetConfig(camera3DConfig);
                        camera->Start();
                        go->AddComponent(camera);
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndPopup();
        }
    }

}