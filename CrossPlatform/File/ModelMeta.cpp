#include "ModelMeta.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "Engine/IO/ObjLoader.h"
namespace CrossPlatform {

    using namespace XenonEngine;

    ModelMeta::~ModelMeta()
    {
        delete m_polygon3D;
        m_polygon3D = nullptr;
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