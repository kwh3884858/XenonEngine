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
		that->m_polygon3D = m_polygon3D;
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
		return true;
	}

	bool Mesh3D::Destroy()
	{
		m_polygon3D.Clear();
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
		if (m_polygon3D.Count() > 0)
		{
			xg::Guid modelGuid = m_polygon3D[0]->GetModelGUID();
			if (modelGuid.isValid() && modelGuid == m_modelId)
			{
				return;
			}
		}
        m_polygon3D = model->GetPolygons();
		m_materials = model->GetMaterials();

        CalculateModelMaxRadius();
    }

    void Mesh3D::CalculateModelMaxRadius()
    {
        assert(m_polygon3D.Count() > 0);
        float maxRadius = 0;
		for (int index = 0 ; index < m_polygon3D.Count(); index++)
		{
			for (int i = 0; i < m_polygon3D[i]->GetNumOfVertex(); i++)
			{
				float radius = (*m_polygon3D[index])[i].m_vertex.x *(*m_polygon3D[index])[i].m_vertex.x + (*m_polygon3D[index])[i].m_vertex.y *(*m_polygon3D[index])[i].m_vertex.y + (*m_polygon3D[index])[i].m_vertex.z *(*m_polygon3D[index])[i].m_vertex.z;
				if (radius > maxRadius)
				{
					maxRadius = radius;
				}
			}
		}

        m_maxRadius = sqrt(maxRadius);
    }

	ComponentType Mesh3D::m_type = ComponentType::ComponentType_Mesh3D;
}