#pragma once
#include "Engine/GameObject.h"
namespace Gameplay
{
	class Player :public XenonEngine::GameObject
	{
	public:
		Player(const Algorithm::String& name);
		virtual ~Player();

		virtual Player* Copy() const;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnTrigger(GameObject* gameobject);
	private:
		bool isFaceRight = true;
	};
}