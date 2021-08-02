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

	Enemy::Enemy(const char* name):
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
        Render2DConfig render2DConfig;
        render2DConfig.m_polygon2D = heroPolygon;
        Render2D* render2D = new Render2D(this);
        render2D->SetConfig(&render2DConfig);
        AddComponent(render2D);

        PlayerPersonality* personality = new PlayerPersonality(this);
        AddComponent(personality);

        Rigidbody2D* const rigid = new Rigidbody2D(this, true, 5, 10);
        AddComponent(rigid);

        BoxCollider2D* collider = new BoxCollider2D(this);
        BoxCollider2DConfig boxCollider2DConfig;
        boxCollider2DConfig.m_isTrigger = true;
        boxCollider2DConfig.m_size = Vector2f(20, 20);
        collider->SetConfig(&boxCollider2DConfig);
        AddComponent(collider);
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