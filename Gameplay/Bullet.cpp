#include "Bullet.h"
#include "Engine/Component/Transform2D.h"
#include "Engine/Component/PlayerPersonality.h"
#include "Engine/Component/Mesh2D.h"
#include "Engine/Component/Rigidbody2D.h"
#include "Engine/Component/BoxCollider2D.h"
#include "CrossPlatform/Polygon/Polygon2D.h"
#include "MathLab/Vector2.h"

namespace Gameplay
{
	using namespace XenonEngine;
	using namespace CrossPlatform;
	using namespace MathLab;

	Bullet::Bullet(const String& name):
		GameObject(name)
	{
	}

	Bullet::~Bullet()
	{
	}

	Bullet* Bullet::Copy() const
	{
		Bullet* that = new Bullet("Untitled");
		return that;
	}

	void Bullet::Start()
	{
		Transform2D* transform = new Transform2D(this);
		AddComponent(transform);

		BoxCollider2D* collider = new BoxCollider2D(this);
		BoxCollider2DConfig boxCollider2DConfig;
		boxCollider2DConfig.m_isTrigger = true;
		boxCollider2DConfig.m_size = Vector2f(2, 2);
		collider->SetConfig(&boxCollider2DConfig);
		AddComponent(collider);

		Rigidbody2D* const rigid = new Rigidbody2D(this, false, 5, 10);
		rigid->SetGravity(false);
		AddComponent(rigid);

		int numOfVertex = 4;
		Vector2f* heroVertex = new Vector2f[numOfVertex];
		heroVertex[0] = Vector2f(5, -5);
		heroVertex[1] = Vector2f(5, 5);
		heroVertex[2] = Vector2f(-5, 5);
		heroVertex[3] = Vector2f(-5, -5);
		Polygon2D* heroPolygon = new Polygon2D(Polygon2D::EState::Enable, CrossPlatform::YELLOW, numOfVertex, heroVertex);
		Mesh2DConfig render2DConfig;
		render2DConfig.m_polygon2D = heroPolygon;
		Mesh2D* render2D = new Mesh2D(this);
		render2D->SetConfig(&render2DConfig);
		AddComponent(render2D);
	}

	void Bullet::Update()
	{
		
	}

	void Bullet::OnTrigger(GameObject* gameobject)
	{
		String name = gameobject->GetName();
		if (name.Find("Player"))
		{
			gameobject->SetState(ObjectState::MarkForDelete);
		}
	}
}