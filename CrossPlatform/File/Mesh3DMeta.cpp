#include "Mesh3DMeta.h"

#include "CrossPlatform/Polygon/Polygon3D.h"
#include "CrossPlatform/Converter/Mesh3DYamlConverter.h"
#include "CrossPlatform/File/Polygon3DMeta.h"

#include "Engine/EngineManager.h"
#include "Engine/Component/Mesh3D.h"
#include "Engine/IO/ObjectImporter.h"

#include <fstream>
#include <filesystem>

namespace CrossPlatform {

    using namespace XenonEngine;
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;

	//void Mesh3DMeta::Load()
	//{
	//	if (!m_mesh)
	//	{
	//		YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
	//		m_mesh =(Mesh3D*) config.as<Mesh3D>().Copy();
	//	}
	//}

	//void Mesh3DMeta::Add()
	//{

	//}

	Mesh3D* Mesh3DMeta::Instantiate() const
	{
		YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
		return new Mesh3D(std::move(config.as<Mesh3D>()));
	}

	void Mesh3DMeta::Clear()
	{
		//if (m_mesh)
		//{
		//	delete m_mesh;
		//	m_mesh = nullptr;
		//}
	}

	void Mesh3DMeta::Save(const XenonObject* data)
	{
		IFileMeta::Save();
		assert(data != nullptr);
		ofstream outputStream(GetFileHeader().GetFilePath().CString());
		YAML::Emitter out(outputStream);
		out << YAML::Node(*(Mesh3D*)data);
		outputStream.close();
	}

	void Mesh3DMeta::Delete()
	{
		IFileMeta::Delete();

		//for (int i = 0; i < m_mesh->m_polygons.Count(); i++)
		//{
		//	xg::Guid polyID = m_mesh->m_polygons[i];
		//	EngineManager::Get().GetFileDatabase().DeleteFile(polyID);
		//}

		//for (int i = 0; i < m_mesh->m_materials.Count(); i++)
		//{
		//	xg::Guid polyID = m_mesh->m_materials[i];
		//	EngineManager::Get().GetFileDatabase().DeleteFile(polyID);
		//}

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			path modelFile(filePath.CString());
			assert(remove(modelFile) == true);
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