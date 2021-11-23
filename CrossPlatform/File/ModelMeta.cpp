#include "ModelMeta.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "Engine/IO/ObjLoader.h"
#include <filesystem>
namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;
    ModelMeta::~ModelMeta()
    {
		for (int i = 0; i < m_polygons.Count(); i++)
		{
			delete m_polygons[i];
			m_polygons[i] = nullptr;
		}

		for (int i = 0; i < m_materials.Count(); i++)
		{
			delete m_materials[i];
			m_materials[i] = nullptr;
		}
    }

	void ModelMeta::Delete()
	{
		for (int i = 0; i < m_polygons.Count(); i++)
		{
			delete m_polygons[i];
			m_polygons[i] = nullptr;
		}

		for (int i = 0; i < m_materials.Count(); i++)
		{
			delete m_materials[i];
			m_materials[i] = nullptr;
		}

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			path modelFile(filePath.CString());
			bool result = remove(modelFile);
			assert(result == true);
		}
		{
			String metaFilePath = filePath + ".metadata";
			path modelMetaFile(metaFilePath.CString());
			bool result = remove(modelMetaFile);
			assert(result == true);
		}
		if (m_materials.Count() != 0)
		{
			path materialFile(filePath.CString());
			materialFile.replace_extension("mtl");
			bool result = remove(materialFile);
			assert(result == true);
		}
	}

	const Vector<Polygon3D*>& ModelMeta::GetPolygons()
    {
        if (m_polygons.Count() == 0)
        {
			LoadModel();
        }
        return m_polygons;
    }

	const CrossPlatform::Vector<Material*>& ModelMeta::GetMaterials()
	{
		if (m_materials.Count() == 0)
		{
			LoadModel();
		}
		return m_materials;
	}

	void ModelMeta::LoadModel()
	{
		bool result = ObjectLoader::Get().LoadObj(m_header.GetFilePath(), m_polygons, m_materials);
		assert(result == true);
		for (int i = 0; i < m_polygons.Count(); i++)
		{
			m_polygons[i]->SetModelGUID(GetFileHeader().GetGUID());
		}
	}

}