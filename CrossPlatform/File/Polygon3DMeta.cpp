#include <filesystem>
#include "Polygon3DMeta.h"
#include "CrossPlatform/Polygon/Polygon3D.h"

namespace CrossPlatform
{
	using namespace Algorithm;

	Polygon3DMeta::~Polygon3DMeta()
	{
		delete m_polygon;
		m_polygon = nullptr;
	}

	void Polygon3DMeta::Delete()
	{
		delete m_polygon;
		m_polygon = nullptr;

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