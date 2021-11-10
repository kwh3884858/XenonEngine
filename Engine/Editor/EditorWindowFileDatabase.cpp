//#include "Engine/Editor/EditorWindowFilaDatabase.h"
#include "EditorWindowFileDatabase.h"

#include "Engine/EngineSyncData.h"

#include "Engine/FileDatabase.h"
namespace XenonEngine
{
    void EditorWindowFileDatabase::UpdateMainWindow(const void* data /*= nullptr*/)
    {
        const EngineSyncData* syncData = static_cast<const EngineSyncData*>(data);
        const FolderMeta* rootFolder = syncData->FolderGetter();
        if (!rootFolder)
        {
            return;
        }
        //// open Dialog Simple
        //if (ImGui::Button("Open File Dialog"))
        //    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".meta", rootFolder->GetFileHeader().GetFilePath().CString());

        //// display
        //if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        //{
        //    // action if OK
        //    if (ImGuiFileDialog::Instance()->IsOk())
        //    {
        //        std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
        //        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
        //        // action
        //    }

        //    // close
        //    ImGuiFileDialog::Instance()->Close();
        //}

        // open Dialog with Pane
        if (ImGui::Button("Open File Dialog with a custom pane"))
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp",
                ".", "", std::bind(&InfosPane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 350.0f, 1, IGFDUserDatas("InfosPane"));

        // display and action if ok
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
                // here convert from string because a string was passed as a userDatas, but it can be what you want
                std::string userDatas;
                if (ImGuiFileDialog::Instance()->GetUserDatas())
                    userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
                auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

                // action
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }
    }
}