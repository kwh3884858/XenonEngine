#include <filesystem>
#include "Polygon3DMeta.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "CrossPlatform/Converter/FileHeaderYamlConverter.h"

namespace CrossPlatform
{
	using namespace Algorithm;

	void Polygon3DMeta::Load()
	{
		if (!m_polygon)
		{
			YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
			m_polygon = new Polygon3D(std::move(config.as<Polygon3D>()));
		}
	}

	void Polygon3DMeta::Clear()
	{
		delete m_polygon;
		m_polygon = nullptr;
	}

	void Polygon3DMeta::Save()
	{

	}

	void Polygon3DMeta::Delete()
	{
		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			std::filesystem::path polygonFile(filePath.CString());
			bool result = remove(polygonFile);
			assert(result == true);
		}
		{
			String metaFilePath = filePath + ".metadata";
			std::filesystem::path polygonMetaFile(metaFilePath.CString());
			bool result = remove(polygonMetaFile);
			assert(result == true);
		}
	}

}