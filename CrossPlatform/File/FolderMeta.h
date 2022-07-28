//  FolderMeta.h
//  XenonEngine
//
//  Created by whkong on 2021-11-11.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "CrossPlatform/File/IFileMeta.h"
namespace CrossPlatform
{
    class FolderMeta :public IFileMeta
    {
    public:
        FolderMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeFolder);  }
		virtual void Load()override {}
		virtual void Save()override;
		virtual void Delete() override;

        void AddIFile(IFileMeta* file) { m_content.Add(file); }
		void RemoveFile(IFileMeta* file) { m_content.Remove(file); }
        IFileMeta* GetFile(int index) const { return m_content[index]; }
        IFileMeta* GetFile(const Algorithm::String& fileName)const;
        int GetFileCount()const { return m_content.Count(); }
        bool CreateFolder() const;

		static void Initialization();

    private:
		static IFileMeta* Create(const Algorithm::String& filePath);
		static IFileMeta* Read(const FileHeader& fileHeader);
        Algorithm::Vector<IFileMeta*> m_content;
	};
}
