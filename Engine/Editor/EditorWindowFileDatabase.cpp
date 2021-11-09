//#include "Engine/Editor/EditorWindowFilaDatabase.h"
#include "EditorWindowFileDatabase.h"

#include "Engine/EngineSyncData.h"

#include "Library/ImGuiFileDialog/ImGuiFileDialog.h"
#include "Engine/FileDatabase.h"
namespace XenonEngine
{
    void EditorWindowFileDatabase::UpdateMainWindow(const void* data /*= nullptr*/)
    {
        const EngineSyncData* syncData = static_cast<const EngineSyncData*>(data);
        const Folder* rootFolder = syncData->FolderGetter();
        if (!rootFolder)
        {
            return;
        }

        ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey");
    }
}