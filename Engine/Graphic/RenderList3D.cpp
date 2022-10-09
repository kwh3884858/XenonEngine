//#include "RenderList3D.h"
//#include "Engine/Component/Mesh3D.h"
//#include "CrossPlatform/Polygon/Polygon3D.h"
//
//using namespace CrossPlatform;
//
//void XenonEngine::RenderList3D::AddMesh3D(const Mesh3D* const mesh)
//{
//	const Algorithm::Vector< CrossPlatform::Polygon3D*>& polygons = mesh->GetPolygon3D();
//	for (const Polygon3D* const polygon : polygons)
//	{
//		for (int i = 0; i < polygon->Count(); i++)
//		{
//			m_vertexIndexList.Add((*polygon)[i]);
//		}
//	}
//	const Algorithm::Vector< CrossPlatform::Material*>& materials = mesh->GetMaterials();
//	for (const Material* const material : materials)
//	{
//		m_materials.Add(*material);
//	}
//
//
//}
//
