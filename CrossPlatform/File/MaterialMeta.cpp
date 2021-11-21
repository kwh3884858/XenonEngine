#include "MaterialMeta.h"
namespace CrossPlatform
{

	MaterialMeta::~MaterialMeta()
	{
		delete m_material;
		m_material = nullptr;
	}

	void MaterialMeta::Delete()
	{
		delete m_material;
		m_material = nullptr;

		const String& filePath = GetFileHeader().GetFilePath();
		if (filePath.Empty())
		{
			return;
		}
		{
			path meterialFile(filePath.CString());
			bool result = remove(meterialFile);
			assert(result == true);
		}
		{
			String metaFilePath = filePath + ".metadata";
			path modelMetaFile(metaFilePath.CString());
			bool result = remove(modelMetaFile);
			assert(result == true);
		}
	}

	//CrossPlatform::Material* MaterialMeta::GetMaterial()
	//{
	//	if (!m_material)
	//	{
	//		m_material = ObjectLoader::Get().LoadObj(m_header.GetFilePath());
	//		const_cast<Polygon3D*>(m_polygon3D)->SetModelGUID(GetFileHeader().GetGUID());
	//	}
	//	return m_material;
	//}

}