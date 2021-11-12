//#include "Engine/Editor/EditorWindowFilaDatabase.h"
#include "EditorWindowFileDatabase.h"

#include "Engine/EngineSyncData.h"

#include "Engine/FileDatabase.h"
#include "Engine/Editor/EditorDatabase.h"
#include "CrossPlatform/File/FolderMeta.h"
#include "Library/IconFontCppHeaders/IconsFontAwesome5.h"
namespace XenonEngine
{

    void EditorWindowFileDatabase::Initialize()
    {
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByExtention, ".cpp", ImVec4(1.0f, 1.0f, 0.0f, 0.9f));
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByExtention, ".h", ImVec4(0.0f, 1.0f, 0.0f, 0.9f));
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByExtention, ".hpp", ImVec4(0.0f, 0.0f, 1.0f, 0.9f));
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByExtention, ".md", ImVec4(1.0f, 0.0f, 1.0f, 0.9f));
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(0.0f, 1.0f, 1.0f, 0.9f), ICON_FA_FILE); // add an icon for the filter type
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByExtention, ".gif", ImVec4(0.0f, 1.0f, 0.5f, 0.9f), "[GIF]"); // add an text for a filter type
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByTypeDir, nullptr, ImVec4(0.5f, 1.0f, 0.9f, 0.9f), ICON_FA_FOLDER); // for all dirs
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByFullName, "doc", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_FA_FILE);
    }

    void EditorWindowFileDatabase::UpdateMainWindow(const void* data /*= nullptr*/)
    {
        const EngineSyncData* syncData = static_cast<const EngineSyncData*>(data);
        const FolderMeta* rootFolder = syncData->FolderGetter();
        if (!rootFolder)
        {
            return;
        }
        EditorDatabase::Get().SetRootFolder(rootFolder);

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
        m_contentBrowser.OpenDialog("ContentBrowser", ICON_FA_FILE "Content Browser", nullptr, rootFolder->GetFileHeader().GetVirtualPath().CString(), true);

            //ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp",
            //    rootFolder->GetFileHeader().GetVirtualPath().CString(), "", std::bind(&InfosPane, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 350.0f, 1, IGFDUserDatas("InfosPane"));
        ImVec2 maxSize = ImVec2((float)1280, (float)720);  // The full display area
        ImVec2 minSize = ImVec2((float)640, (float)360);  // Half the display area

        // display and action if ok
        if (m_contentBrowser.Display("ContentBrowser", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
        {
            if (m_contentBrowser.IsOk())
            {
                std::string filePathName = m_contentBrowser.GetFilePathName();
                std::string filePath = m_contentBrowser.GetCurrentPath();
                std::string filter = m_contentBrowser.GetCurrentFilter();
                // here convert from string because a string was passed as a userDatas, but it can be what you want
                std::string userDatas;
                if (m_contentBrowser.GetUserDatas())
                    userDatas = std::string((const char*)m_contentBrowser.GetUserDatas());
                auto selection = m_contentBrowser.GetSelection(); // multiselection

                // action
            }
            // close
            m_contentBrowser.Close();
        }

        if (ImGui::Button(ICON_FA_FILE_IMPORT " Add new file"))
        {
            const char *filters = "Model (*.obj){.obj},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},World (*.world){.world}";
            m_fileImporter.OpenDialog("ChooseFile", "Add file", nullptr, (rootFolder->GetFileHeader().GetFilePath()+"\\").CString());
        }
        // display and action if ok
        if (m_fileImporter.Display("ChooseFile", ImGuiWindowFlags_NoCollapse, minSize, maxSize))
        {
            if (m_fileImporter.IsOk())
            {
                std::string filePathName = m_fileImporter.GetFilePathName();
                std::string filePath = m_fileImporter.GetCurrentPath();
                std::string filter = m_fileImporter.GetCurrentFilter();
                // here convert from string because a string was passed as a userDatas, but it can be what you want
                std::string userDatas;
                if (m_fileImporter.GetUserDatas())
                    userDatas = std::string((const char*)m_fileImporter.GetUserDatas());
                auto selection = m_fileImporter.GetSelection(); // multiselection

                // action
            }
            // close
            m_fileImporter.Close();
        }
    }
}