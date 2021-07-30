#pragma once
#include "Engine/GameObject.h"
namespace Gameplay
{
	class Enemy:public XenonEngine::GameObject
	{
	public:
		Enemy(const char* name);
		virtual ~Enemy();

		virtual GameObject* Copy()const;
		virtual void Start() override;
		virtual void Update()override;
		virtual void Destroy()override;
	private:

	};


}