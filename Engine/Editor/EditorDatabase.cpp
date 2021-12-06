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
        return const_cast<FileDatabase*>(m_fileDatabase)->CreateFolder(virtualPath);
    }

    Algorithm::String EditorDatabase::ConvertToRealPath(const Algorithm::String& virtualPath) const
    {
        return m_fileDatabase->ConvertToRealPath(virtualPath);
    }

	void EditorDatabase::Delete(const Algorithm::String& virtualPath)
	{
		return const_cast<FileDatabase*>(m_fileDatabase)->DeleteFile(virtualPath);
	}

}