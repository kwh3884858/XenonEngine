#include "Cube3D.h"

#include "Engine/IO/ObjLoader.h"

#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Mesh3D.h"
#include "Engine/FileManager/FileManager.h"

namespace Gameplay
{
    using namespace XenonEngine;
    using namespace CrossPlatform;
    using namespace MathLab;

    Cube3D::Cube3D(const String& name) :
        GameObject(name)
    {
    }

    Cube3D::~Cube3D()
    {
    }

    Cube3D* Cube3D::Copy() const
    {
        Cube3D* that = new Cube3D("Untitled");
        return that;
    }

    void Cube3D::Start()
    {
        Transform3D* transform = new Transform3D(this);
        transform->SetPosition(Vector3f(0, 0, 3));
        AddComponent(transform);

        String applicationPath = FileManager::Get().GetApplicationPath();
        int pos = applicationPath.IndexOf("XenonEngine.exe");
        String cubeObjPath = applicationPath.Substring(0, pos);
        cubeObjPath.Append("KeQing.obj"/*"cubeTest.obj"*/);
        Mesh3DConfig config;
        config.m_polygon3D = ObjectLoader::Get().LoadObj(cubeObjPath);
        Mesh3D* mesh3D = new Mesh3D(this);
        mesh3D->SetConfig(&config);
        AddComponent(mesh3D);

    }

    void Cube3D::Update()
    {

    }

    void Cube3D::OnTrigger(GameObject* gameobject)
    {

    }
}