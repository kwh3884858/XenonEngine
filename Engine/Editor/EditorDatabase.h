#pragma once
#include "CrossPlatform/XenonSingleton.h"
#include "CrossPlatform/File/IFileMeta.h"

#include <cassert>

namespace CrossPlatform
{
    class FolderMeta;
}
namespace XenonEngine
{
    class EditorDatabase:public CrossPlatform::XenonSingleton<EditorDatabase>
    {
    public:

        const CrossPlatform::FolderMeta* GetRootFolder()const { assert(m_rootFolder != nullptr); return m_rootFolder; }
        void SetRootFolder(const CrossPlatform::FolderMeta* root) { m_rootFolder = root; }

        CrossPlatform::FolderMeta* GetFolder(const Algorithm::String& virtualPath) const;
        bool CreateFolder(const Algorithm::String& virtualPath) const;
        Algorithm::String ConvertVirtualPath(const Algorithm::String& virtualPath);
    private:
        const CrossPlatform::FolderMeta* m_rootFolder = nullptr;

    };

}