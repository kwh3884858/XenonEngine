#include "Mesh3D.h"
#include "Transform3D.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "Engine/Graphic/Graphic3D.h"
#include "Engine/FileManager/FileManager.h"

#include <cassert>

#include "Engine/EngineManager.h"
#include "CrossPlatform/File/ModelMeta.h"

namespace XenonEngine
{
    using namespace CrossPlatform;

	IComponent* Mesh3D::Copy(GameObject*const gameObject) const
	{
		Mesh3D* that = new Mesh3D(gameObject);
		that->m_polygons = m_polygons;
		that->m_materials = m_materials;
		that->m_modelId = m_modelId;
        that->m_maxRadius = m_maxRadius;
		return that;
	}

	bool Mesh3D::Start()
	{
        LoadModel();
        return true;
	}

	bool Mesh3D::Update()
	{
		if (m_requestToReload)
		{
			LoadModel();
		}
		return true;
	}

	bool Mesh3D::Destroy()
	{
		m_polygons.Clear();
		m_materials.Clear();
		return true;
	}

    void Mesh3D::SetModelGuid(const xg::Guid& modelGuid)
    {
        m_modelId = modelGuid;
    }

    void Mesh3D::LoadModel()
    {
        if (!m_modelId.isValid())
        {
            return;
        }
        const IFileMeta* dataRoot = EngineManager::Get().GetFileDatabase().GetFile(m_modelId);
        if (dataRoot == nullptr)
        {
            m_modelId = xg::Guid();
            return;
        }

        ModelMeta* model = (ModelMeta*)(dataRoot);
		if (m_polygons.Count() > 0)
		{
			xg::Guid modelGuid = m_polygons[0]->GetModelGUID();
			if (modelGuid.isValid() && modelGuid == m_modelId)
			{
				return;
			}
		}
        m_polygons = model->GetPolygons();
		m_materials = model->GetMaterials();

        CalculateModelMaxRadius();
    }

    void Mesh3D::CalculateModelMaxRadius()
    {
        assert(m_polygons.Count() > 0);
        float maxRadius = 0;
		for (int index = 0 ; index < m_polygons.Count(); index++)
		{
			for (int i = 0; i < m_polygons[index]->GetNumOfVertex(); i++)
			{
				float radius = (*m_polygons[index])[i].m_vertex.x *(*m_polygons[index])[i].m_vertex.x + (*m_polygons[index])[i].m_vertex.y *(*m_polygons[index])[i].m_vertex.y + (*m_polygons[index])[i].m_vertex.z *(*m_polygons[index])[i].m_vertex.z;
				if (radius > maxRadius)
				{
					maxRadius = radius;
				}
			}
		}

        m_maxRadius = sqrt(maxRadius);
    }

	void Mesh3D::RequestReloadModel()
	{
		m_requestToReload = true;
	}

	ComponentType Mesh3D::m_type = ComponentType::ComponentType_Mesh3D;
}