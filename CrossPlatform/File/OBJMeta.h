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
		OBJMeta(const FileHeader& header) :IFileMeta(header) { }
		virtual ~OBJMeta() override = default;

		virtual void OnImport() override;
		virtual XenonObject* Instantiate() const override { return nullptr; }
		virtual void Clear() {}

		// Save to hard drive as a data file
		virtual void Save(const XenonObject* data = nullptr);

		// Delete data file from hard drive
		virtual void Delete();
	};
}