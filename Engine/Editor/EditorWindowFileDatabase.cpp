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
        m_contentBrowser.SetFileStyle(IGFD_FileStyleByFullName, ".doc", ImVec4(0.9f, 0.2f, 0.0f, 0.9f), ICON_FA_FILE);
    }

    void EditorWindowFileDatabase::UpdateMainWindow(const void* data /*= nullptr*/)
    {
        const EngineSyncData* syncData = static_cast<const EngineSyncData*>(data);
        FileDatabase* database = (FileDatabase*) syncData->DatabaseGetter();
        if (!database)
        {
            return;
        }
        EditorDatabase::Get().SetFileDatabase(database);

        m_contentBrowser.OpenDialog("ContentBrowser", ICON_FA_FILE "Content Browser", nullptr, database->GetRootFolder()->GetFileHeader().GetVirtualPath().CString(), true);

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

        if (ImGui::Button(ICON_FA_ANGRY " Add new file"))
        {
            const char *filters = "Model (*.obj){.obj},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},World (*.world){.world},Folder{}";
            m_fileImporter.OpenDialog("ChooseFile", ICON_FA_AMBULANCE "Add file", filters, (database->GetRootFolder()->GetFileHeader().GetFilePath()+"\\.").CString());
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
                database->AddFile(filePathName.c_str());
                // action
            }
            // close
            m_fileImporter.Close();
        }
    }
}