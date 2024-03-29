#include "Mesh3D.h"
#include "Transform3D.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "Engine/Graphic/Graphic3D.h"
#include "Engine/FileManager/FileManager.h"

#include <cassert>

#include "Engine/EngineManager.h"
#include "CrossPlatform/File/Mesh3DMeta.h"
#include "CrossPlatform/File/Polygon3DMeta.h"
#include "CrossPlatform/File/MaterialMeta.h"

namespace XenonEngine
{
    using namespace CrossPlatform;
	using namespace xg;

	Mesh3D::Mesh3D(Mesh3D&& that):
		IComponent(that.m_gameobject),
		m_polygons(that.m_polygons),
		m_vertexs(that.m_vertexs),
		m_normals(that.m_normals),
		m_uv(that.m_uv),
		m_materials(that.m_materials),
		m_cachePolygons(that.m_cachePolygons),
		m_cacheMaterials(that.m_cacheMaterials),
		m_maxRadius(that.m_maxRadius),
		m_requestToReload(that.m_requestToReload)
	{
	}

	Mesh3D::~Mesh3D()
	{
		for(auto cache : m_cachePolygons)
		{
			delete cache.second;
			cache.second = nullptr;
		}
		for (auto cache : m_cacheMaterials)
		{
			delete cache.second;
			cache.second = nullptr;
		}
	}

	IComponent* Mesh3D::Copy(GameObject* const gameObject) const
	{
		Mesh3D* that = new Mesh3D(gameObject);
		that->m_polygons = m_polygons;
		that->m_vertexs = m_vertexs;
		that->m_materials = m_materials;
		that->m_normals = m_normals;
		that->m_uv = m_uv;
		that->m_maxRadius = m_maxRadius;
		that->m_requestToReload = m_requestToReload;
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

	const CrossPlatform::Triangle3D Mesh3D::operator[](int index)
	{
		int finder = index;
		for (const Guid& guid : m_polygons)
		{
			assert(index >= 0); //index is out of array;
			const Polygon3D& polygon = GetPolygon3D(guid);
			if (finder < polygon.Count())
			{
				const Polygon3D::TriangleIndex& triangle = polygon[finder];
				Triangle3D result;
				result[0].m_vertex = MathLab::ConvertFromNonHomogeneous(m_vertexs[triangle[0].m_vertexIndex]);
				result[0].m_normal = MathLab::ConvertFromNonHomogeneous(m_normals[triangle[0].m_normalIndex]);
				result[0].m_uv = m_uv[triangle[0].m_textureCoordinateIndex];
				result[1].m_vertex = MathLab::ConvertFromNonHomogeneous(m_vertexs[triangle[1].m_vertexIndex]);
				result[1].m_normal = MathLab::ConvertFromNonHomogeneous(m_normals[triangle[1].m_normalIndex]);
				result[1].m_uv = m_uv[triangle[1].m_textureCoordinateIndex];
				result[2].m_vertex = MathLab::ConvertFromNonHomogeneous(m_vertexs[triangle[2].m_vertexIndex]);
				result[2].m_normal = MathLab::ConvertFromNonHomogeneous(m_normals[triangle[2].m_normalIndex]);
				result[2].m_uv = m_uv[triangle[2].m_textureCoordinateIndex];
				result.m_materialIndex = triangle.m_materialIndex;

				return result;
			}
			else
			{
				finder -= polygon.Count();
			}
		}
		assert(true == false);  //index is out of array; too big index;
		return Triangle3D();
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
			const Polygon3D* polygon = polygonMeta->Instantiate();
			m_cachePolygons[guid] = polygon;
			return *polygon;
		}
	}

	int Mesh3D::TriangleCount()
	{
		int result = 0;
		for (const xg::Guid& guid : m_polygons)
		{
			const CrossPlatform::Polygon3D& polygon = GetPolygon3D(guid);
			result += polygon.Count();
		}
		return result;
	}

	CrossPlatform::Material& Mesh3D::GetMaterial(int index)
	{
		Guid guid = m_materials[index];

		if (m_cacheMaterials.find(guid) != m_cacheMaterials.end())
		{
			return *(const_cast<Material*>(m_cacheMaterials.at(guid)));
		}
		else
		{
			const MaterialMeta* materialMeta = (MaterialMeta*)EngineManager::Get().GetFileDatabase().GetFile(guid);
			const Material* material = materialMeta->Instantiate();
			m_cacheMaterials[guid] = material;
			return *const_cast<Material*>(material);
		}
	}

	void Mesh3D::SetPolygonGuid(int index, xg::Guid guid)
	{
		m_polygons[index] = guid;
		if (m_cachePolygons.find(guid) != m_cachePolygons.end())
		{
			const Polygon3DMeta* polygonMeta = (Polygon3DMeta*)EngineManager::Get().GetFileDatabase().GetFile(guid);
			const Polygon3D* polygon = polygonMeta->Instantiate();
			m_cachePolygons[guid] = polygon;
		}
	}

	void Mesh3D::SetMaterialGuid(int index, xg::Guid guid)
	{
		m_materials[index] = guid;
		if (m_cacheMaterials.find(guid) != m_cacheMaterials.end())
		{
			const MaterialMeta* materialMeta = (MaterialMeta*)EngineManager::Get().GetFileDatabase().GetFile(guid);
			const Material* material = materialMeta->Instantiate();
			m_cacheMaterials[guid] = material;
		}
	}

	ComponentType Mesh3D::m_type = ComponentType::ComponentType_Mesh3D;
}