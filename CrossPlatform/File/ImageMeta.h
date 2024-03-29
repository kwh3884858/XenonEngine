//  ImageMeta.h
//  XenonEngine
//
//  Created by whkong on 2021-12-11.
//  Copyright (c) 2021 whkong. All rights reserved.

#pragma once
#include "CrossPlatform/File/IFileMeta.h"
#include "CrossPlatform/Image/Image.h"

namespace XenonEngine
{
    class FileDatabase;
    class GameObjectWorld;
}

namespace CrossPlatform
{
	//class Image;
    class ImageMeta :public IFileMeta
    {
    public:
        friend class XenonEngine::FileDatabase;
		ImageMeta(const FileHeader& header) :IFileMeta(header) { }
		virtual ~ImageMeta()override = default;

		// Load into memory
		//virtual void Add() override;
		virtual Image* Instantiate() const override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save(const XenonObject* data = nullptr) override;

		// Delete data file from hard drive
		virtual void Delete() override;

		//Image* GetImage();
    private:
		Image* ImportImageFileIntoFolderAsNativeVersion() const;
		//Image* m_image = nullptr;
    };
}