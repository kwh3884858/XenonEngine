#include "Enemy.h"
#include "Engine/Component/Transform2D.h"
#include "Engine/Component/PlayerPersonality.h"
#include "Engine/Component/Render2D.h"
#include "Engine/Component/Rigidbody2D.h"
#include "Engine/Component/BoxCollider2D.h"
#include "CrossPlatform/Polygon2D.h"

#include "MathLab/Vector2f.h"

namespace Gameplay
{
	using namespace XenonEngine;
    using namespace CrossPlatform;
    using namespace MathLab;

	Enemy::Enemy()
	{
	}

	Enemy::Enemy(const char* name):
		GameObject(name)
	{
	}

	Enemy::~Enemy()
	{
	}
    GameObject* Enemy::Copy() const
    {
        Enemy* that = new Enemy("Enemy");
        return that;
    }
    void Enemy::Start()
	{
        enemy = new GameObject("Enemy");
        Transform2D* transform = new Transform2D(enemy);
        transform->AddPosition(Vector2f(500, 300));
        enemy->AddComponent(transform);

        int numOfVertex = 4;
        Vector2f* heroVertex = new Vector2f[numOfVertex];
        heroVertex[0] = Vector2f(10, -10);
        heroVertex[1] = Vector2f(10, 10);
        heroVertex[2] = Vector2f(-10, 10);
        heroVertex[3] = Vector2f(-10, -10);
        Polygon2D* heroPolygon = new Polygon2D(Polygon2D::EState::Enable, CrossPlatform::BLUE, numOfVertex, heroVertex);
        Render2DConfig render2DConfig;
        render2DConfig.m_polygon2D = heroPolygon;
        Render2D* render2D = new Render2D(enemy);
        render2D->SetConfig(&render2DConfig);
        enemy->AddComponent(render2D);

        PlayerPersonality* personality = new PlayerPersonality(enemy);
        enemy->AddComponent(personality);

        Rigidbody2D* const rigid = new Rigidbody2D(enemy, false, 5, 10);
        enemy->AddComponent(rigid);

        BoxCollider2D* collider = new BoxCollider2D(enemy);
        BoxCollider2DConfig boxCollider2DConfig;
        boxCollider2DConfig.m_isTrigger = true;
        boxCollider2DConfig.m_size = Vector2f(20, 20);
        collider->SetConfig(&boxCollider2DConfig);
        enemy->AddComponent(collider);

        physics2D->AddGameObject(enemy);
        world->AddGameObject(enemy);
	}
}