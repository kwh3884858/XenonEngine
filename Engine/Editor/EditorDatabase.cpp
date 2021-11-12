#include "EditorDatabase.h"
#include "Engine/FileDatabase.h"

namespace XenonEngine
{
    using namespace Algorithm;
    using namespace CrossPlatform;

    CrossPlatform::FolderMeta* EditorDatabase::GetFolder(const Algorithm::String& virtualPath) const
    {
        return m_fileDatabase->GetFolderByVirtualPath(virtualPath);
    }

    CrossPlatform::FolderMeta* EditorDatabase::CreateFolder(const Algorithm::String& virtualPath) const
    {
        return m_fileDatabase->CreateFolderByVirtualPath(virtualPath);
    }

    Algorithm::String EditorDatabase::ConvertToRealPath(const Algorithm::String& virtualPath)
    {
        return m_fileDatabase->ConvertToRealPath(virtualPath);
    }

}