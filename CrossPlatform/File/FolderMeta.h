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
        FolderMeta(const FileHeader& header) :IFileMeta(header) { }
		virtual ~FolderMeta() override = default;

		virtual void OnLoad() override;

		// For folder, is it impossible to generate a related instance.
		virtual XenonObject* Instantiate() const override { assert(true == false); return nullptr; }

		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save(const XenonObject* data = nullptr) override;

		// Delete data file from hard drive
		virtual void Delete() override;


        void AddIFile(IFileMeta* file) { m_content.Add(file); }
		void RemoveFile(IFileMeta* file) { m_content.Remove(file); }
        IFileMeta* GetFile(int index) const { return m_content[index]; }
        IFileMeta* GetFile(const Algorithm::String& fileName)const;
        int GetFileCount()const { return m_content.Count(); }
        bool CreateFolder() const;

		//static void Registration();

    private:
		//static IFileMeta* Create(const Algorithm::String& filePath);
		//static IFileMeta* Read(const FileHeader& fileHeader);
        Algorithm::Vector<IFileMeta*> m_content;
	};
}
