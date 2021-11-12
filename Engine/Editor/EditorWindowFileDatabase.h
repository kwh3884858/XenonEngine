#pragma once

#include "Engine/Editor/XenonBaseWindow.h"
#include "Library/ImGuiFileDialog/ImGuiFileDialog.h"

namespace XenonEngine
{

    static bool canValidateDialog = false;
    inline void InfosPane(const char *vFilter, IGFDUserDatas vUserDatas, bool *vCantContinue) // if vCantContinue is false, the user cant validate the dialog
    {
        ImGui::TextColored(ImVec4(0, 1, 1, 1), "Infos Pane");
        ImGui::Text("Selected Filter : %s", vFilter);
        if (vUserDatas)
            ImGui::Text("UserDatas : %s", vUserDatas);
        ImGui::Checkbox("if not checked you cant validate the dialog", &canValidateDialog);
        if (vCantContinue)
            *vCantContinue = canValidateDialog;
    }

	class EditorWindowFileDatabase : public XenonBaseWindow{
        WINDOW_DECLARE_NAME("File Database")
        WINDOW_DECLARE_TYPE(Window_Type::SingleWindow)
    public:
        virtual void Initialize() override;
        virtual void Shutdown() override {}


    protected:
        virtual void UpdateMainWindow(const void* data = nullptr) override;
	private:
        ImGuiFileDialog m_contentBrowser;
        ImGuiFileDialog m_fileImporter;
	};

}