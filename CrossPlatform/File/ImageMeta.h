//  ImageMeta.h
//  XenonEngine
//
//  Created by whkong on 2021-12-11.
//  Copyright (c) 2021 whkong. All rights reserved.

#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace XenonEngine
{
    class FileDatabase;
    class GameObjectWorld;
}

namespace CrossPlatform
{    
	class Image;
    class ImageMeta :public IFileMeta
    {
    public:
        friend class XenonEngine::FileDatabase;
		ImageMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeImage); }
        virtual ~ImageMeta()override;

		// Load into memory
		virtual void Load() override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save() override;

		// Delete data file from hard drive
		virtual void Delete() override;

		Image* GetImage();
    private:
		void Clear();
		Image* m_image = nullptr;
    };

}