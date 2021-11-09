#include "Enemy.h"
#include "Engine/Component/Transform2D.h"
#include "Engine/Component/PlayerPersonality.h"
#include "Engine/Component/Mesh2D.h"
#include "Engine/Component/Rigidbody2D.h"
#include "Engine/Component/BoxCollider2D.h"
#include "CrossPlatform/Polygon/Polygon2D.h"
#include "Engine/Timer/XenonTimer.h"
#include "Engine/GameObjectWorldManager.h"
#include "Engine/GameObjectWorld.h"
#include "Engine/Physics/Force2D.h"

#include "MathLab/Vector2.h"
#include "Gameplay/Bullet.h"
#include "Engine/EngineManager.h"

namespace Gameplay
{
	using namespace XenonEngine;
    using namespace CrossPlatform;
    using namespace MathLab;
    using namespace XenonPhysics;

	Enemy::Enemy(const String& name):
		GameObject(name)
	{
	}

	Enemy::~Enemy()
	{
	}

    Enemy* Enemy::Copy() const
    {
        Enemy* that = new Enemy("Untitled");
        return that;
    }

    void Enemy::Start()
	{
        Transform2D* transform = new Transform2D(this);
        transform->AddPosition(Vector2f(500, 300));
        AddComponent(transform);

        int numOfVertex = 4;
        Vector2f* heroVertex = new Vector2f[numOfVertex];
        heroVertex[0] = Vector2f(10, -10);
        heroVertex[1] = Vector2f(10, 10);
        heroVertex[2] = Vector2f(-10, 10);
        heroVertex[3] = Vector2f(-10, -10);
        Polygon2D* heroPolygon = new Polygon2D(Polygon2D::EState::Enable, CrossPlatform::BLUE, numOfVertex, heroVertex);
        Mesh2DConfig render2DConfig;
        render2DConfig.m_polygon2D = heroPolygon;
        Mesh2D* render2D = new Mesh2D(this);
        render2D->SetConfig(&render2DConfig);
        AddComponent(render2D);

        PlayerPersonality* personality = new PlayerPersonality(this);
        AddComponent(personality);

        Rigidbody2D* const rigid = new Rigidbody2D(this, false, 5, 10);
        AddComponent(rigid);

        BoxCollider2D* collider = new BoxCollider2D(this);
        BoxCollider2DConfig boxCollider2DConfig;
        boxCollider2DConfig.m_isTrigger = true;
        boxCollider2DConfig.m_size = Vector2f(20, 20);
        collider->SetConfig(&boxCollider2DConfig);
        AddComponent(collider);
	}

	void Enemy::Update()
	{
		GameObject* gameobject = EngineManager::Get().GetWorldManager().GetCurrentWorld()->GetGameObject("Player");
		if (gameobject!= nullptr)
		{
			float currentTime = XenonTimer::Get().GetTime();
			if (currentTime - m_lastTime > 5000.0f)
			{
				Transform2D* tranform = GetComponent<Transform2D>();
				Transform2D* playerTransform = gameobject->GetComponent<Transform2D>();
				Vector2f positionVector = playerTransform->GetPosition() - tranform->GetPosition();
                Vector2f posVecNormal = positionVector.Normalize();
				// New bullet
				Bullet* bullet = new Bullet("Bullet");
                EngineManager::Get().GetWorldManager().GetCurrentWorld()->AddGameObject(bullet);
				Transform2D* bulletTransform = bullet->GetComponent<Transform2D>();
                bulletTransform->SetPosition(tranform->GetPosition() + posVecNormal * 20);
                Rigidbody2D* rigid = bullet->GetComponent<Rigidbody2D>();
                Force2D jumpForce;
                jumpForce.fvalue = 5000.0f;
                jumpForce.m_forceDirection = posVecNormal;
                rigid->AddForce(jumpForce);

				m_lastTime = currentTime;
			}
		}
	}

    void Enemy::OnTrigger(GameObject* gameobject)
    {
        String name = gameobject->GetName();
        if (name.Find("Bullet"))
        {
            SetState(ObjectState::MarkForDelete);
        }
    }
}