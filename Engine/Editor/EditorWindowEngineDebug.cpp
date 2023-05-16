//#include "Engine/Editor/EditorWindowFilaDatabase.h"
#include "EditorWindowEngineDebug.h"

#include "Engine/EngineSyncData.h"
#include "Engine/Graphic/Graphic3D.h"

#include "Library/imgui/imgui.h"
#include "Library/IconFontCppHeaders/IconsFontAwesome5.h"
namespace XenonEngine
{

    void EditorWindowEngineDebug::Initialize()
    {

    }

    void EditorWindowEngineDebug::UpdateMainWindow(const void* data /*= nullptr*/)
    {
        const EngineSyncData* syncData = static_cast<const EngineSyncData*>(data);
		Graphic3D* graph3D = (Graphic3D*) syncData->Graphic3DGetter();
        if (!graph3D)
        {
            return;
        }

		ImGui::Text("Remove Back Face: %s", graph3D->IsRemoveBackFaces() ? "True" : "False");
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_ARROW_CIRCLE_LEFT " Toggle Remove Back Faces"))
		{
			graph3D->ToggleRemoveBackFaces();
		}

    }
}