#pragma once
#include "Engine/GameObject.h"
namespace Gameplay
{
	class Enemy :public XenonEngine::GameObject
	{
	public:
		Enemy(const char* name);
		virtual ~Enemy();

		virtual Enemy* Copy() const;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnTrigger(GameObject* gameobject);
	private:

	};


}