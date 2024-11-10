#include <filesystem>
#include <fstream>

#include "MaterialMeta.h"
#include "CrossPlatform/Material/Material.h"
#include "CrossPlatform/Converter/MaterialYamlConverter.h"

namespace CrossPlatform
{
	using namespace Algorithm;
	using namespace std;


	//void MaterialMeta::Load()
	//{
	//	if (!m_material)
	//	{
	//		YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
	//		m_material = new Material(std::move(config.as<Material>()));
	//	}
	//}

	CrossPlatform::Material* MaterialMeta::Instantiate() const
	{
		YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
		return new Material(std::move(config.as<Material>()));
	}

	void MaterialMeta::Clear()
	{
		//if (m_material)
		//{
		//	delete m_material;
		//	m_material = nullptr;
		//}
	}

	//void MaterialMeta::Save()
	//{
	//	IFileMeta::Save();

	//	ofstream outputStream(GetFileHeader().GetFilePath().CString());
	//	YAML::Emitter out(outputStream);
	//	out << YAML::Node(*m_material);
	//	outputStream.close();
	//}

	void MaterialMeta::Save(const XenonObject* material)
	{
		IFileMeta::Save();

		ofstream outputStream(GetFileHeader().GetFilePath().CString());
		YAML::Emitter out(outputStream);
		out << YAML::Node(*(Material*)material);
		outputStream.close();
	}

	void MaterialMeta::Delete()
	{
		//delete m_material;
		//m_material = nullptr;

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			std::filesystem::path meterialFile(filePath.CString());
			assert(remove(meterialFile) == true);
		}
		{
			String metaFilePath = filePath + ".metadata";
			std::filesystem::path modelMetaFile(metaFilePath.CString());
			assert(remove(modelMetaFile) == true);
		}
	}

}