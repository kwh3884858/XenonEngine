#pragma once
#include "Engine/GameObject.h"
namespace Gameplay
{
	class Bullet :public XenonEngine::GameObject
	{
	public:
		Bullet(const char* name);
		virtual ~Bullet();

		virtual Bullet* Copy() const;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnTrigger(GameObject* gameobject);
	private:

	};
}