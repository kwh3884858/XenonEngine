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
        AddComponent(transform);

        Camera3DConfig camera3DConfig;
        camera3DConfig.m_lookAt = Vector3f(0, 0, -10);
        camera3DConfig.m_viewDistance = Vector2f(1, 1);
        camera3DConfig.m_fov = 1;
        camera3DConfig.m_viewport = Vector2f(Database::Get().engineConfig.m_width, Database::Get().engineConfig.m_height);
        Camera3D* camera3D = new Camera3D(this);
        camera3D->SetConfig(&camera3DConfig);
        AddComponent(camera3D);
    }

    void CameraObject3D::Update()
    {
        Transform3D* trans = GetComponent<Transform3D>();
        float xAxisDelta = 0;
        float yAxisDelta = 0;
        static float velocity = 0.1;
        if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_W))
        {
            yAxisDelta = 1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_S))
        {
            yAxisDelta = -1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_A))
        {
            xAxisDelta = -1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_D))
        {
            xAxisDelta = 1;
        }
        trans->AddPosition(Vector2f(xAxisDelta * velocity, yAxisDelta*velocity));
    }

    void CameraObject3D::OnTrigger(GameObject* gameobject)
    {

    }
}