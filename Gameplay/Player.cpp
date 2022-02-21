#include "Player.h"

#include "Engine/Component/Transform2D.h"
#include "Engine/Component/PlayerPersonality.h"
#include "Engine/Component/Mesh2D.h"
#include "Engine/Component/Rigidbody2D.h"
#include "Engine/Component/BoxCollider2D.h"

#include "Engine/GameObjectWorldManager.h"
#include "Engine/GameObjectWorld.h"
#include "Engine/Timer/XenonTimer.h"
#include "Engine/Physics/Force2D.h"
#include "Engine/IO/InputSystem.h"
#include "CrossPlatform/XenonKey.h"

#include "CrossPlatform/Polygon/Polygon2D.h"
#include "CrossPlatform/Database.h"

#include "MathLab/MathLib.h"
#include "MathLab/MathLabDefinition.h"

#include "Gameplay/Bullet.h"

#include <cstdio>
#include "Engine/EngineManager.h"
namespace Gameplay
{
	using namespace XenonEngine;
	using namespace CrossPlatform;
	using namespace MathLab;
	using namespace XenonPhysics;
	Player::Player(const String& name):
		GameObject(name)
	{
	}

	Player::~Player()
	{
	}

	Player * Player::Copy() const
	{
		Player* that = new Player("Untitled");
		return that;
	}

	void Player::Start()
	{
		Transform2D* transform = new Transform2D(this);
		transform->AddPosition(Vector2f(400, 300));
		AddComponent(transform);

		int numOfVertex = 4;
		Vector2f* heroVertex = new Vector2f[numOfVertex];
		heroVertex[0] = Vector2f(10, -10);
		heroVertex[1] = Vector2f(10, 10);
		heroVertex[2] = Vector2f(-10, 10);
		heroVertex[3] = Vector2f(-10, -10);
		Polygon2D* heroPolygon = new Polygon2D(Polygon2D::EState::Enable, CrossPlatform::GREEN, numOfVertex, heroVertex);
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
		boxCollider2DConfig.m_isTrigger = false;
		boxCollider2DConfig.m_size = Vector2f(20, 20);
		collider->SetConfig(&boxCollider2DConfig);
		AddComponent(collider);
	}

	void Player::Update()
	{
		PlayerPersonality* personlity = GetComponentPointer<PlayerPersonality>();
		Transform2D* playerTransform = GetComponentPointer<Transform2D>();
		Rigidbody2D* rigid = GetComponentPointer<Rigidbody2D>();

		static bool shouldPrintPlayerData = false;
		if (shouldPrintPlayerData)
		{
			printf("( %f , %f ) \n", playerTransform->GetPosition().x, playerTransform->GetPosition().y);
		}

		//unsigned int width = Database::Get().engineConfig.m_width;
		//unsigned int height = Database::Get().engineConfig.m_height;

		float xAxisDelta = 0;
		Vector2f axis = InputSystem::Get().GetAxis();
		if (MathLab::Abs(axis.x) > 0.1f)
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

		if (MathLab::Abs(xAxisDelta) > EPSILON)
		{
			if (xAxisDelta > 0)
			{
				isFaceRight = true;
			}
			else
			{
				isFaceRight = false;
			}
		}

		float velocity = personlity->GetVelocity();
		playerTransform->AddPosition(Vector2f(xAxisDelta * velocity, 0));

		if (InputSystem::Get().GetStickButton(0) || InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_SPACE))
		{
			Force2D jumpForce;
			jumpForce.fvalue = personlity->GetJumpForce();
			jumpForce.m_forceDirection = Vector2f(0, 1);
			rigid->AddForce(jumpForce);
		}
		if (InputSystem::Get().GetStickButton(3))
		{
			GameObject* newbullet = new Bullet("Bullet");
			GameObjectWorld* world = EngineManager::Get().GetWorldManager().GetCurrentWorld();
			world->AddGameObject(newbullet);

			Transform2D* bulletTransform = newbullet->GetComponentPointer<Transform2D>();
			Vector2f bulletPos = playerTransform->GetPosition() + Vector2f((isFaceRight ? 1 : -1) * 30.0f, 0.0f);
			bulletTransform->SetPosition(bulletPos);
			Rigidbody2D* bulletRigid = newbullet->GetComponentPointer<Rigidbody2D>();
			Force2D jumpForce;
			jumpForce.fvalue = personlity->GetBulletForce();
			if (isFaceRight)
			{
				jumpForce.m_forceDirection = Vector2f(1, 0);
			}
			else
			{
				jumpForce.m_forceDirection = Vector2f(-1, 0);
			}

			bulletRigid->AddForce(jumpForce);
		}
	}

	void Player::OnTrigger(GameObject* gameobject)
	{
		gameobject;
	}

}