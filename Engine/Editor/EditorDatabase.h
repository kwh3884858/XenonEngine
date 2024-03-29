#pragma once
#include "CrossPlatform/XenonSingleton.h"
#include "CrossPlatform/File/IFileMeta.h"

#include <cassert>
#include "crossguid/guid.hpp"

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
        void SetFileDatabase(XenonEngine::FileDatabase* database) { m_fileDatabase = database; }

        CrossPlatform::FolderMeta* GetFolder(const Algorithm::String& virtualPath) const;
        CrossPlatform::FolderMeta* CreateFolder(const Algorithm::String& virtualPath);
        Algorithm::String ConvertToRealPath(const Algorithm::String& virtualPath) const;
		void Delete(const Algorithm::String& virtualPath);
        void ShowInExplorer(const Algorithm::String& virtualPath)const;
        void RequestRefreshContent();

		CrossPlatform::IFileMeta* GetFileMeta(xg::Guid guid) const;
    private:
         XenonEngine::FileDatabase* m_fileDatabase = nullptr;
	};

}