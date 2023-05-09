//  Model.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace XenonEngine
{
	class Mesh3D;
	class ObjectLoader;
}
namespace CrossPlatform {
    class Mesh3DMeta :public IFileMeta<Mesh3D>
    {
    public:
		friend class XenonEngine::ObjectLoader;

        Mesh3DMeta(const FileHeader& header) :IFileMeta(header) { }
		virtual ~Mesh3DMeta() override = default;

		// Add into content browser. create meta for file
		//virtual void Add() override;

		// Instantiate a new instance
		virtual XenonEngine::Mesh3D* Instantiate()override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save() override;

		// Delete data file from hard drive
		virtual void Delete() override;

	private:
		//XenonEngine::Mesh3D* m_mesh = nullptr;
    };
}