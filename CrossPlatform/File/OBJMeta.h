//  ImageMeta.h
//  XenonEngine
//
//  Created by whkong on 2021-12-11.
//  Copyright (c) 2021 whkong. All rights reserved.

#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace CrossPlatform {

	class OBJMeta :public IFileMeta
	{
	public:
		OBJMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeMesh3D); }
		virtual ~OBJMeta() override = default;

		// Load into memory
		virtual void Load() override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save() override;

		// Delete data file from hard drive
		virtual void Delete() override;
	};
}