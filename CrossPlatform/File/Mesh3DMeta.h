//  Model.h
//  XenonEngine
//
//  Created by whkong on 2021-11-10.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace CrossPlatform {
	using Algorithm::Vector;
    class Polygon3DMeta;
	class MaterialMeta;
	class Polygon3D;
	class Material;
    class Mesh3DMeta :public IFileMeta
    {
    public:
        Mesh3DMeta(const FileHeader& header) :IFileMeta(header) { m_header.SetFileType(FileType::FileTypeMesh); }
        virtual ~Mesh3DMeta() override;
		virtual void Delete() override;
        const Vector<Polygon3D*>& GetPolygons();
		const Vector<Material*>& GetMaterials();
	private:
		void Clear();
		void LoadModel();
		Vector<Polygon3DMeta*> m_polygons;
		Vector<MaterialMeta*> m_materials;

		Vector<Polygon3D*> m_cachePolygons;
		Vector<Material*> m_cacheMaterials;
    };
}