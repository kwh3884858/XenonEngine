#include "EditorDatabase.h"
#include "Engine/FileDatabase.h"



namespace XenonEngine
{
    using namespace Algorithm;
    using namespace CrossPlatform;

    CrossPlatform::FolderMeta* EditorDatabase::GetFolder(const Algorithm::String& virtualPath) const
    {
        return m_fileDatabase->GetFolder(virtualPath);
    }

    CrossPlatform::FolderMeta* EditorDatabase::CreateFolder(const Algorithm::String& virtualPath)
    {
        return const_cast<FileDatabase*>(m_fileDatabase)->GetOrCreateFolder(virtualPath);
    }

    Algorithm::String EditorDatabase::ConvertToRealPath(const Algorithm::String& virtualPath) const
    {
        return m_fileDatabase->ConvertToRealPath(virtualPath);
    }

	void EditorDatabase::Delete(const Algorithm::String& virtualPath)
	{
		return const_cast<FileDatabase*>(m_fileDatabase)->DeleteFile(virtualPath);
	}

#ifdef _WIN32
#include <windows.h>
#include <ShellAPI.h>
#endif

	void EditorDatabase::ShowInExplorer(const Algorithm::String& virtualPath) const
	{
        String absolutedPath = m_fileDatabase->ConvertToRealPath(virtualPath);
#ifdef _WIN32
        HINSTANCE result = ShellExecuteA(nullptr, "open", absolutedPath.CString(), nullptr, nullptr, SW_SHOWDEFAULT);
        assert(result > (HINSTANCE)HINSTANCE_ERROR);
#endif
	}

	void EditorDatabase::RequestRefreshContent()
	{
        return m_fileDatabase->RefreshContent();
	}

}