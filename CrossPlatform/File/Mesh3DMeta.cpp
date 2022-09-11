#include "Mesh3DMeta.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "Engine/IO/ObjLoader.h"
#include "CrossPlatform/Converter/Mesh3DYamlConverter.h"

#include <fstream>
#include <filesystem>
namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;

	void Mesh3DMeta::Load()
	{
		if (!m_mesh)
		{
			YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
			m_mesh = config.as<Mesh3D>().Copy();
		}
	}

	void Mesh3DMeta::Clear()
	{
		if (m_mesh)
		{
			delete m_mesh;
			m_mesh = nullptr;
		}
	}

	void Mesh3DMeta::Save()
	{
		IFileMeta::Save();

		ofstream outputStream(GetFileHeader().GetFilePath().CString());
		YAML::Emitter out(outputStream);
		out << YAML::Node(*m_mesh);
		outputStream.close();
	}

	void Mesh3DMeta::Delete()
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


	//void Mesh3DMeta::LoadModel()
	//{
	//	Clear();
	//	bool result = ObjectLoader::Get().LoadObj(m_header.GetFilePath(), m_polygons, m_materials);
	//	assert(result == true);
	//	for (int i = 0; i < m_polygons.Count(); i++)
	//	{
	//		m_polygons[i]->SetModelGUID(GetFileHeader().GetGUID());
	//	}
	//}

}