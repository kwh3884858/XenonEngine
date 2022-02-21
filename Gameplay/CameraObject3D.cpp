#include "CameraObject3D.h"

#include "Engine/IO/ObjLoader.h"

#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Camera3D.h"
#include "CrossPlatform/Database.h"
#include "Engine/IO/InputSystem.h"
#include "CrossPlatform/XenonKey.h"

namespace Gameplay
{
    using namespace XenonEngine;
    using namespace CrossPlatform;
    using namespace MathLab;

    CameraObject3D::CameraObject3D(const String& name) :
        GameObject(name)
    {
    }

    CameraObject3D::~CameraObject3D()
    {
    }

    CameraObject3D* CameraObject3D::Copy() const
    {
        CameraObject3D* that = new CameraObject3D("Untitled");
        return that;
    }

    void CameraObject3D::Start()
    {
        Transform3D* transform = new Transform3D(this);
        transform->SetPosition(Vector3f(0, 0, -1));
        transform->SetRotation(Vector3f(0, -90, 0));
        AddComponent(transform);

        Camera3DConfig camera3DConfig;
        camera3DConfig.m_fov = 90;
        camera3DConfig.m_viewport = Vector2i(Database::Get().engineConfig.m_width, Database::Get().engineConfig.m_height);
        camera3DConfig.m_farClipZ = 1000;
        camera3DConfig.m_nearClipZ = 1;
        Camera3D* camera3D = new Camera3D(this);
        camera3D->SetConfig(camera3DConfig);
        AddComponent(camera3D);
    }

    void CameraObject3D::Update()
    {
        Transform3D* trans = GetComponentPointer<Transform3D>();
        float xAxisDelta = 0;
        float yAxisDelta = 0;
        float zAxisDelta = 0;
        float velocity = 0.1f;

        if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_W))
        {
            xAxisDelta = 1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_S))
        {
            xAxisDelta = -1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_Q))
        {
            yAxisDelta = 1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_E))
        {
            yAxisDelta = -1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_A))
        {
            zAxisDelta = 1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_D))
        {
            zAxisDelta = -1;
        }
        Vector3f delatPos(xAxisDelta * velocity, yAxisDelta*velocity, zAxisDelta* velocity);
        TVector3f tDeltaPos(delatPos);
        tDeltaPos = tDeltaPos * trans->GetRotationTranformMatrix();
        trans->AddPosition(tDeltaPos.GetVetor());

        float headingDelta = 0;
        float ElevationDelta = 0;
        if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LEFT))
        {
            headingDelta = -1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_RIGHT))
        {
            headingDelta = 1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_UP))
        {
            ElevationDelta = 1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_DOWN))
        {
            ElevationDelta = -1;
        }
        trans->AddRotation(Vector3f(ElevationDelta,headingDelta,0));
    }

    void CameraObject3D::OnTrigger(GameObject* gameobject)
    {
		gameobject;
    }
}