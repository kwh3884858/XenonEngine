#include "Engine/GameplayMain.h"

#include "Engine/Primitive/Primitive2D.h"
#include "Engine/IO/InputSystem.h"
#include "Engine/Physics/Physics2D.h"
#include "Engine/Physics/Force2D.h"
#include "Engine/VirtualMachine/XenonCompiler.h"

#include "CrossPlatform/Interface/IInput.h"
#include "CrossPlatform/XenonKey.h"
#include "CrossPlatform/Database.h"
#include "CrossPlatform/SColorRGBA.h"
#include "CrossPlatform/Polygon2D.h"

#include "MathLab/Vector3.h"
#include "MathLab/MathLib.h"
#include "MathLab/MathLabDefinition.h"
#include <cmath> // sin, cos

#include "Engine/GameObjectWorld.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Transform2D.h"
#include "Engine/Component/Render2D.h"
#include "Engine/Component/PlayerPersonality.h"
#include "Engine/Component/Rigidbody2D.h"
#include "Engine/Component/BoxCollider2D.h"

namespace Gameplay {
    using MathLab::Vector3f;
    using MathLab::Vector2f;
    using CrossPlatform::Database;
    using CrossPlatform::Polygon2D;
    using Primitive::Primitive2D;

    using XenonEngine::XenonCompiler;
    using XenonEngine::InputSystem;
    using XenonPhysics::Physics2D;
    using XenonPhysics::Force2D;

    using XenonEngine::Render2D;
    using XenonEngine::Render2DConfig;

    using XenonEngine::GameObject;
    using XenonEngine::GameObjectWorld;
    using XenonEngine::Transform2D;
    using XenonEngine::PlayerPersonality;
    using XenonEngine::Rigidbody2D;
    using XenonEngine::BoxCollider2D;
    using XenonEngine::BoxCollider2DConfig;
    using XenonEngine::ComponentType;

    GameObject* player;
    GameObject* ground;
    Physics2D* physics2D;

    XenonCompiler* compiler = nullptr;
    void GameplayInitialize()
    {
        GameObjectWorld::Get().Initialize();

        physics2D = new Physics2D;


        {
            player = new GameObject("Player");
            GameObjectWorld::Get().AddGameObject(player);

            Transform2D* transform = new Transform2D(player);
            transform->AddPosition(Vector2f(400, 300));
            player->AddComponent(transform);

            int numOfVertex = 4;
            Vector2f* heroVertex = new Vector2f[numOfVertex];
            heroVertex[0] = Vector2f(10, 0);
            heroVertex[1] = Vector2f(10, 20);
            heroVertex[2] = Vector2f(-10, 20);
            heroVertex[3] = Vector2f(-10, 0);
            Polygon2D* heroPolygon = new Polygon2D(Polygon2D::EState::Enable, CrossPlatform::WHITE, numOfVertex, heroVertex);
            Render2DConfig render2DConfig;
            render2DConfig.m_polygon2D = heroPolygon;
            Render2D* render2D = new Render2D(player);
            render2D->SetConfig(&render2DConfig);
            player->AddComponent(render2D);

            PlayerPersonality* personality = new PlayerPersonality(player);
            player->AddComponent(personality);

            Rigidbody2D* const rigid = new Rigidbody2D(player, false, 5, 10);
            physics2D->AddRigidbody2D(rigid);
            player->AddComponent(rigid);

            BoxCollider2D* collider = new BoxCollider2D(player);
            BoxCollider2DConfig boxCollider2DConfig;
            boxCollider2DConfig.m_isTrigger = false;
            boxCollider2DConfig.m_size = Vector2f(20, 20);
            collider->SetConfig(&boxCollider2DConfig);
            player->AddComponent(collider);
        }

        {
            ground = new GameObject("Ground");

            Transform2D* transform = new Transform2D(ground);
            transform->AddPosition(Vector2f(400, 100));
            ground->AddComponent(transform);

            BoxCollider2D* collider = new BoxCollider2D(ground);
            BoxCollider2DConfig boxCollider2DConfig;
            boxCollider2DConfig.m_isTrigger = false;
            boxCollider2DConfig.m_size = Vector2f(400, 40);
            ground->AddComponent(collider);

            int numOfVertex = 4;
            Vector2f* heroVertex = new Vector2f[numOfVertex];
            heroVertex[0] = Vector2f(200, -10);
            heroVertex[1] = Vector2f(200, 10);
            heroVertex[2] = Vector2f(-200, 10);
            heroVertex[3] = Vector2f(-200, -10);
            Polygon2D* heroPolygon = new Polygon2D(Polygon2D::EState::Enable, CrossPlatform::WHITE, numOfVertex, heroVertex);
            Render2DConfig render2DConfig;
            render2DConfig.m_polygon2D = heroPolygon;
            Render2D* render2D = new Render2D(ground);
            render2D->SetConfig(&render2DConfig);
            ground->AddComponent(render2D);


        }
        compiler = new XenonCompiler;
        compiler->Initialize();
    }

    void GameplayMain()
    {
        PlayerPersonality* personlity = player->GetComponent<PlayerPersonality>();
        Transform2D* transform = player->GetComponent<Transform2D>();

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
            transform->SetPosition(Vector2f(400, 300));
        }

        unsigned int width = Database::Get().engineConfig.m_width;
        unsigned int height = Database::Get().engineConfig.m_height;

        float xAxisDelta = 0;
        Vector2f axis = InputSystem::Get().GetAxis();
        if (MathLab::abs(axis.x) > 0.1f)
        {
            xAxisDelta = axis.x;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_A))
        {
            xAxisDelta = -1;
        }
        else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_D))
        {
            xAxisDelta = 1;
        }

        float velocity = personlity->GetVelocity();
        transform->AddPosition(Vector2f(xAxisDelta * velocity, 0));

        Rigidbody2D* rigid = player->GetComponent<Rigidbody2D>();
        if (InputSystem::Get().GetStickButton(0) || InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_SPACE))
        {
            Force2D jumpForce;
            jumpForce.fvalue = personlity->GetJumpForce();
            jumpForce.m_forceDirection = Vector2f(0, 1);
            rigid->AddForce(jumpForce);
        }

        physics2D->FixedUpdate();
        {
            Render2D* render2D = player->GetComponent<Render2D>();
            render2D->Update();
        }
        {
            Render2D* render2D = ground->GetComponent<Render2D>();
            render2D->Update();
        }
    }

    void GameplayShutdown()
    {
        delete compiler;

        GameObjectWorld::Get().Shutdown();
    }

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


}