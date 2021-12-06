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
    class FileDatabase;
    class EditorDatabase:public CrossPlatform::XenonSingleton<EditorDatabase>
    {
    public:

        //const CrossPlatform::FolderMeta* GetRootFolder()const { assert(m_rootFolder != nullptr); return m_rootFolder; }
        void SetFileDatabase(const XenonEngine::FileDatabase* database) { m_fileDatabase = database; }

        CrossPlatform::FolderMeta* GetFolder(const Algorithm::String& virtualPath) const;
        CrossPlatform::FolderMeta* CreateFolder(const Algorithm::String& virtualPath);
        Algorithm::String ConvertToRealPath(const Algorithm::String& virtualPath) const;
		void Delete(const Algorithm::String& virtualPath);
        void ShowInExplorer(const Algorithm::String& virtualPath)const;
    private:
         const XenonEngine::FileDatabase* m_fileDatabase = nullptr;
	};

}