#include "Mesh3D.h"
#include "Transform3D.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "Engine/Graphic/Graphic3D.h"
#include "Engine/FileManager/FileManager.h"

#include <cassert>

#include "Engine/EngineManager.h"
#include "CrossPlatform/File/Mesh3DMeta.h"
#include "CrossPlatform/File/Polygon3DMeta.h"

namespace XenonEngine
{
    using namespace CrossPlatform;
	using namespace xg;

	IComponent* Mesh3D::Copy(GameObject*const gameObject) const
	{
		Mesh3D* that = new Mesh3D(gameObject);
		that->m_polygons = m_polygons;
		that->m_materials = m_materials;
		that->m_uv = m_uv;
        that->m_maxRadius = m_maxRadius;
		return that;
	}

	//bool Mesh3D::Start()
	//{
 //       //LoadModel();
 //       return true;
	//}

	//bool Mesh3D::Update()
	//{
	//	//if (m_requestToReload)
	//	//{
	//	//	LoadModel();
	//	//}
	//	return true;
	//}

	//bool Mesh3D::Destroy()
	//{
	//	//m_polygons.Clear();
	//	//m_materials.Clear();
	//	return true;
	//}

  //  void Mesh3D::SetModelGuid(const xg::Guid& modelGuid)
  //  {
  //      m_modelId = modelGuid;
  //  }

  //  void Mesh3D::LoadModel()
  //  {
  //      if (!m_modelId.isValid())
  //      {
  //          return;
  //      }
  //      const IFileMeta* dataRoot = EngineManager::Get().GetFileDatabase().GetFile(m_modelId);
  //      if (dataRoot == nullptr)
  //      {
  //          m_modelId = xg::Guid();
  //          return;
  //      }

  //      Mesh3DMeta* model = (Mesh3DMeta*)(dataRoot);
		//if (m_polygons.Count() > 0)
		//{
		//	xg::Guid modelGuid = m_polygons[0]->GetModelGUID();
		//	if (modelGuid.isValid() && modelGuid == m_modelId)
		//	{
		//		return;
		//	}
		//}
  //      m_polygons = model->GetPolygons();
		//m_materials = model->GetMaterials();

  //      CalculateModelMaxRadius();
  //  }

    void Mesh3D::CalculateModelMaxRadius()
    {
        assert(m_vertexs.Count() > 0);
        float maxDoubleRadius = 0;
		for (const Vector3f& vertex : m_vertexs)
		{
			float doubleRadius = vertex.DoubleMagnitude();
			if (doubleRadius > maxDoubleRadius)
			{
				maxDoubleRadius = doubleRadius;
			}
		}

        m_maxRadius = sqrt(maxDoubleRadius);
    }

	//void Mesh3D::RequestReloadModel()
	//{
	//	m_requestToReload = true;
	//}

	//const Algorithm::Vector<const CrossPlatform::Polygon3D*> Mesh3D::GetPolygon3Ds() const
	//{
	//	Vector<const Polygon3D*> output;
	//	for (const xg::Guid& id : m_polygons)
	//	{
	//		const Polygon3DMeta* const polygonMeta = dynamic_cast<const Polygon3DMeta*>( EngineManager::Get().GetFileDatabase().GetFile(id) );
	//		const Polygon3D* polygon = polygonMeta->GetPolygon3D();
	//		output.Add(polygon);
	//	}
	//}

	bool Mesh3D::IsValid() const
	{
		return m_polygons.Count() != 0;
	}

	const CrossPlatform::Vertex3D Mesh3D::operator[](int index) 
	{
		int finder = index;
		for (const Guid& guid : m_polygons)
		{
			assert(index >= 0); //index is out of array;
			const Polygon3D& polygon = GetPolygon3D(guid);
			if (finder < polygon.Count())
			{
				const Polygon3D::VertexIndex& vertex = polygon[finder];
				Vertex3D result;
				result.m_vertex = MathLab::ConvertFromNonHomogeneous(m_vertexs[vertex.m_vertexIndex]);
				result.m_normal = MathLab::ConvertFromNonHomogeneous(m_normals[vertex.m_normalIndex]);
				result.m_uv = m_uv[vertex.m_textureCoordinateIndex];
				result.m_materialIndex = vertex.m_materialIndex;

				return result;
			}
			else
			{
				finder -= polygon.Count();
			}
		}
		assert(true == false);  //index is out of array; too big index;
	}

	const CrossPlatform::Polygon3D& Mesh3D::GetPolygon3D(const Guid& guid) 
	{
		if (m_cachePolygons.find(guid) != m_cachePolygons.end())
		{
			return *(m_cachePolygons.at(guid));
		}
		else
		{
			const Polygon3DMeta* polygonMeta = (Polygon3DMeta*) EngineManager::Get().GetFileDatabase().GetFile(guid);
			const Polygon3D* polygon = polygonMeta->GetPolygon3D();
			m_cachePolygons[guid] = polygon;
			return *polygon;
		}
	}

	int Mesh3D::VertexCount()
	{
		int result = 0;
		for (const xg::Guid& guid : m_polygons)
		{
			const CrossPlatform::Polygon3D& polygon = GetPolygon3D(guid);
			result += polygon.Count();
		}
		return result;
	}

	ComponentType Mesh3D::m_type = ComponentType::ComponentType_Mesh3D;
}