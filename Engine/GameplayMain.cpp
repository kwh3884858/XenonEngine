#include "Engine/GameplayMain.h"

#include "Engine/Graphic/Graphic2D.h"

#include "Engine/Physics/Force2D.h"
#include "Engine/VirtualMachine/XenonCompiler.h"

#include "CrossPlatform/Interface/IInput.h"

#include "CrossPlatform/SColorRGBA.h"
#include "CrossPlatform/Polygon/Polygon2D.h"

#include "MathLab/Vector3.h"
#include <cmath> // sin, cos
#include <cstdio> // printf

#include "Engine/GameObjectWorldManager.h"
#include "Engine/GameObjectWorld.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Transform2D.h"
#include "Engine/Component/Transform3D.h"
#include "Engine/Component/Mesh2D.h"
#include "Engine/Component/PlayerPersonality.h"
#include "Engine/Component/Rigidbody2D.h"
#include "Engine/Component/BoxCollider2D.h"
#include "Engine/Component/Camera3D.h"

#include "Engine/IO/InputSystem.h"
#include "CrossPlatform/XenonKey.h"
#include "CrossPlatform/Database.h"

#include "Gameplay/Enemy.h"
#include "Gameplay/Player.h"
#include "Gameplay/Cube3D.h"
#include "Gameplay/CameraObject3D.h"
#include "Gameplay/SceneDirectionLight.h"

namespace Gameplay {
    using MathLab::Vector3f;
    using MathLab::Vector2f;
    using CrossPlatform::Database;
    using CrossPlatform::Polygon2D;
    using XenonEngine::Graphic2D;

    using XenonEngine::XenonCompiler;
    using XenonEngine::InputSystem;
    using XenonPhysics::Force2D;

    using XenonEngine::Mesh2D;
    using XenonEngine::Mesh2DConfig;

    using XenonEngine::GameObject;
    using XenonEngine::GameObjectWorld;
    using XenonEngine::GameObjectWorldManager;
    using XenonEngine::Transform2D;
    using XenonEngine::Transform3D;
    using XenonEngine::PlayerPersonality;
    using XenonEngine::Rigidbody2D;
    using XenonEngine::BoxCollider2D;
    using XenonEngine::BoxCollider2DConfig;
    using XenonEngine::ComponentType;
    using XenonEngine::Camera3D;

    GameObjectWorld* world;

    XenonCompiler* compiler = nullptr;
    void GameplayInitialize()
    {
        GameObjectWorldManager::Get().Initialize();
        world = GameObjectWorldManager::Get().CreateGameWorld("Shooting2D");
        {
            Player* player = new Player("Player");
            //world->AddGameObject(player);
        }

        {
			GameObject* ground = new GameObject("Ground");

            Transform2D* transform = new Transform2D(ground);
            transform->AddPosition(Vector2f(400, 100));
            ground->AddComponent(transform);

            BoxCollider2D* collider = new BoxCollider2D(ground);
            BoxCollider2DConfig boxCollider2DConfig;
            boxCollider2DConfig.m_isTrigger = false;
            boxCollider2DConfig.m_size = Vector2f(400, 20);
            collider->SetConfig(&boxCollider2DConfig);
            ground->AddComponent(collider);

            int numOfVertex = 4;
            Vector2f* heroVertex = new Vector2f[numOfVertex];
            //heroVertex[0] = Vector2f(100, -10);
            //heroVertex[1] = Vector2f(100, 10);
            //heroVertex[2] = Vector2f(-100, 10);
            //heroVertex[3] = Vector2f(-100, -10);

            //heroVertex[0] = Vector2f(100, -10);
            //heroVertex[1] = Vector2f(40, 10);
            //heroVertex[2] = Vector2f(-40, 10);
            //heroVertex[3] = Vector2f(-100, -10);

            heroVertex[0] = Vector2f(100, 0);
            heroVertex[1] = Vector2f(0, 100);
            heroVertex[2] = Vector2f(-100, 0);
            heroVertex[3] = Vector2f(0, -100);
            Polygon2D* heroPolygon = new Polygon2D(Polygon2D::EState::Enable, CrossPlatform::WHITE, numOfVertex, heroVertex);
            Mesh2DConfig render2DConfig;
            render2DConfig.m_polygon2D = heroPolygon;
            Mesh2D* render2D = new Mesh2D(ground);
            render2D->SetConfig(&render2DConfig);
            ground->AddComponent(render2D);

            //world->AddGameObject(ground);
        }

   //     {
			//Enemy* enemy = new Enemy("Enemy");
   //         world->AddGameObject(enemy);
   //     }

        {
            Cube3D* cube = new Cube3D("Cube3D");
            world->AddGameObject(cube);
        }

        {
            CameraObject3D* camera3D = new CameraObject3D("CameraObject3D");
            world->AddGameObject(camera3D);
        }

        {
            SceneDirectionLight* directionLight = new SceneDirectionLight("SceneDirectionLight");
            world->AddGameObject(directionLight);
        }

        compiler = new XenonCompiler;
        compiler->Initialize();
    }


    void GameplayMain()
    {
        if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LCONTROL) &&
            InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_C))
        {
            printf("Recompile\n");
            compiler->Recompile();
        }
		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LCONTROL) &&
			InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_R))
		{
			printf("Respawn Player\n");
			GameObject* player = world->GetGameObject("Player");
			if (!player)
			{
				player = new Player("Player");
				world->AddGameObject(player);
			}
			Transform2D* playerTransform = player->GetComponent<Transform2D>();
			playerTransform->SetPosition(Vector2f(400, 300));

			Rigidbody2D* rigid = player->GetComponent<Rigidbody2D>();
			rigid->SetVelocity(Vector2f(0, 0));

			compiler->RunScript();
		}
        
        //if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_F))
        //{
        //    GameObject* cube = world->GetGameObject("Cube3D");
        //    Transform3D* cubeTrans = cube->GetComponent<Transform3D>();

        //    GameObject* camera3D = world->GetGameObject("CameraObject3D");
        //    Transform3D* cameraTrans = camera3D->GetComponent<Transform3D>();
        //    Camera3D* camera = camera3D->GetComponent<Camera3D>();

        //    Vector3f lookAt = cubeTrans->GetPosition() - cameraTrans->GetPosition();
        //    camera->SetLookAt(lookAt);
        //}

        //Graphic2D::Get().DrawLine(Vector2i(0, 90), Vector2i(800, 90), CrossPlatform::YELLOW);
        //Graphic2D::Get().DrawLine(Vector2i(0, 110), Vector2i(800, 110), CrossPlatform::YELLOW);
        GameObjectWorldManager::Get().Update();
    }

    void GameplayShutdown()
    {
        delete compiler;

        GameObjectWorldManager::Get().Shutdown();
    }

	/*
    void Donut()
    {
        Vector3f cameraPosition();
        static int tmpdegree = 90;

        Vector3f lightDirection(0, 1, 0);
        int donutRadius = 120;
        int circleRadius = 40;

        float degree = 0;    //(0 ~ 360)

        float X = 0;
        float Y = 0;
        float Z = 0;

        for (int thetaDegree = 0; thetaDegree < 360; thetaDegree += 5)
        {
            Vector3f circle;
            Vector3f circleNormal;
            float posX = 0;
            float posY = 0;
            float posZ = 0;

            float theta = thetaDegree * TO_RADIAN;
            circle.x = donutRadius + circleRadius * sin(theta);
            circle.y = circleRadius * cos(theta);
            circleNormal.x = sin(theta);
            circleNormal.y = cos(theta);

            //To donut, rotate with y-axis
            //(x,y,z) * |cos  , 0 ,sin|
            //          |0    ,1  ,0  |       
            //          |-sin ,0  ,cos|

            for (int phiDegree = 0; phiDegree < 360; phiDegree += 5)
            {
                Vector3f donutVertex = MathLab::RotateYAxis(circle, (float)phiDegree);
                Vector3f donutVertexNormal = MathLab::RotateYAxis(circleNormal, (float)phiDegree);
                //float phi = phiDegree * TO_RADIAN;
                //X = posX * cos(phi) - posZ * sin(phi);
                //Y = posY;
                //Z = posY * sin(phi) + posZ * cos(phi);

                //local
                //Vector3 donutVertex(X, Y, Z);

                donutVertex = MathLab::RotateXAxis(donutVertex, (float)tmpdegree);
                donutVertexNormal = MathLab::RotateXAxis(donutVertexNormal, (float)tmpdegree);
                //local to world
                donutVertex = MathLab::RotateZAxis(donutVertex, tmpdegree);
                donutVertexNormal = MathLab::RotateZAxis(donutVertexNormal, (float)tmpdegree);
                donutVertex += Vector3f(0, 0, -170);

                //local to view 


                //view to projection
                int screenX = 0;
                int screenY = 0;

                if (donutVertex.z >= -1)
                {
                    continue;
                }
                float reciprocalOfZ = -1 / donutVertex.z;

                unsigned int screenWidth = Database::Get().engineConfig.m_width;
                unsigned int screenHight = Database::Get().engineConfig.m_height;
                // Rasterized
                if (Database::Get().engineConfig.m_isPerspectiveProjection)
                {
                    screenX = (int)screenWidth / 2 + donutVertex.x * reciprocalOfZ * 100;
                    screenY = (int)screenHight / 2 - donutVertex.y * reciprocalOfZ * 100;
                }
                else {
                    screenX = (int)donutVertex.x + screenWidth / 2;
                    screenY = (int)-(donutVertex.y) + screenHight / 2;
                }

                // Z buffer
                //float curretZBuffer = m_windowDrawer->GetFrameBuffer()->GetZBuffer(screenX, screenY);
                float curretZBuffer = static_cast<float>(Primitive2D::Get().GetZbuffer(Vector2i(screenX, screenY)));

                if (curretZBuffer < reciprocalOfZ)
                {
                    float L = donutVertexNormal.dot(lightDirection);
                    L += 0.5f;
                    if (L > 0)
                    {
                        L *= 255; //scale up to 255
                        L += 20;
                        L = MathLab::clamp(L, 0.0f, 255.0f);
                        Primitive2D::Get().SetZBuffer(Vector2i(screenX, screenY), static_cast<unsigned int>(reciprocalOfZ));
                        Primitive2D::Get().DrawPixel(screenX, screenY, CrossPlatform::SColorRGBA(L, L, L));
                    }
                }
            }
        }

        tmpdegree += 1;
    }
	*/

}