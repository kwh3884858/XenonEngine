#include <filesystem>
#include <fstream>

#include "Polygon3DMeta.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "CrossPlatform/Converter/Polygon3DYamlConverter.h"

namespace CrossPlatform
{
	using namespace Algorithm;
	using namespace std;
	using namespace std::filesystem;

	//void Polygon3DMeta::Add()
	//{

	//}

	YAML::Polygon3D* Polygon3DMeta::Instantiate() const 
{
		YAML::Node config = YAML::LoadFile(m_header.GetFilePath().CString());
		return new Polygon3D(std::move(config.as<Polygon3D>()));
	}

	void Polygon3DMeta::Clear()
	{
		//delete m_polygon;
		//m_polygon = nullptr;
	}

	//void Polygon3DMeta::Save()
	//{

	//}

	void Polygon3DMeta::Save(const XenonObject* polygon)
	{
		IFileMeta::Save();
		assert(polygon != nullptr);
		ofstream outputStream(GetFileHeader().GetFilePath().CString());
		YAML::Emitter out(outputStream);
		out << YAML::Node(*(Polygon3D*)polygon);
		outputStream.close();
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