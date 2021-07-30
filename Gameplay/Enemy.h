#pragma once
#include "Engine/GameObject.h"
namespace Gameplay
{
	class Enemy:public XenonEngine::GameObject
	{
	public:
		Enemy();
		~Enemy();

	private:
		virtual void Start() override;
		virtual void Update()override;
		virtual void Destroy()override;
	};

	Enemy::Enemy()
	{
	}

	Enemy::~Enemy()
	{
	}
}