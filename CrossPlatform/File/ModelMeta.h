//  Model.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace CrossPlatform {

    class Polygon3D;
    class ModelMeta :public IFileMeta
    {
    public:
        ModelMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeModel); }
        virtual ~ModelMeta() override;
		virtual void Delete() override;
        const Polygon3D* GetPolygon()const;
    private:
        mutable const Polygon3D* m_polygon3D = nullptr;
    };
}