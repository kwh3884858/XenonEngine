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
        that->m_maxRadius = m_maxRadius;
		return that;
	}

	bool Mesh3D::Start()
	{
        LoadModel();

        Graphic3D::Get().AddGameobjectToRenderList(GetGameObject());
        return true;
	}

	bool Mesh3D::Update()
	{
		return true;
	}

	bool Mesh3D::Destroy()
	{
        Graphic3D::Get().RemoveGameobjectFromRenderList(GetGameObject());
		delete m_polygon3D;
		m_polygon3D = nullptr;
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

        const ModelMeta* model = static_cast<const ModelMeta*>(dataRoot);
        m_polygon3D = model->GetPolygon();

        CalculateModelMaxRadius();
    }

    void Mesh3D::CalculateModelMaxRadius()
    {
        assert(m_polygon3D != nullptr);
        float maxRadius = 0;
        for (int i = 0; i < m_polygon3D->GetNumOfVertex(); i++)
        {
            float radius = (*m_polygon3D)[i].m_vertex.x *(*m_polygon3D)[i].m_vertex.x + (*m_polygon3D)[i].m_vertex.y *(*m_polygon3D)[i].m_vertex.y + (*m_polygon3D)[i].m_vertex.z *(*m_polygon3D)[i].m_vertex.z;
            if (radius > maxRadius)
            {
                maxRadius = radius;
            }
        }
        m_maxRadius = sqrt(maxRadius);
    }

	ComponentType Mesh3D::m_type = ComponentType::ComponentType_Mesh3D;
}