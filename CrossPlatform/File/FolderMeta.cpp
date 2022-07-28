#include "FolderMeta.h"
#include <filesystem>

#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

namespace CrossPlatform
{
    using namespace std;
    using namespace std::filesystem;
    using namespace Algorithm;



	IFileMeta* FolderMeta::Create(const Algorithm::String& filePath)
	{
		xg::Guid guid = xg::newGuid();
		FolderMeta* folderMeta = new FolderMeta(FileHeader(FileType::FileTypeFolder, filePath, guid));
		return folderMeta;
	}

	void FolderMeta::Delete()
	{
		for (int i =0 ; i < m_content.Count(); i++)
		{
			m_content[i]->Delete();
			RemoveFile(m_content[i]);
		}
		assert(m_content.Count() == 0); 
	}

	CrossPlatform::IFileMeta* FolderMeta::GetFile(const Algorithm::String& fileName) const
    {
        for (int i = 0 ; i < m_content.Count(); i++)
        {
            if (m_content[i]->GetFileHeader().GetFileName() == fileName)
            {
                return m_content[i];
            }
        }
        return nullptr;
    }

    bool FolderMeta::CreateFolder() const
    {
        path folderPath(GetFileHeader().GetFilePath().CString());
        if (!exists(folderPath))
        {
            create_directory(folderPath);
        }
        return true;
    }

	CrossPlatform::IFileMeta* FolderMeta::Initialization()
	{
		RegisterFileFacotry(FileType::FileTypeFolder, Create);
	}

}