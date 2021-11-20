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
        delete m_polygon3D;
        m_polygon3D = nullptr;
    }

	void ModelMeta::Delete()
	{
		delete m_polygon3D;
		m_polygon3D = nullptr;

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
	}

	const CrossPlatform::Polygon3D* ModelMeta::GetPolygon()const
    {
        if (!m_polygon3D)
        {
            m_polygon3D = ObjectLoader::Get().LoadObj(m_header.GetFilePath());
			const_cast<Polygon3D*>(m_polygon3D)->SetModelGUID(GetFileHeader().GetGUID());
        }
        return m_polygon3D;
    }

}