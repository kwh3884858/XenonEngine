#pragma once

#include "IComponent.h"

namespace XenonEngine
{
	class Camera3DController final :public IComponent
	{
	public:
		static const float PI;

		Camera3DController(GameObject* gameobject = nullptr) :
			IComponent(gameobject) {}
		virtual ~Camera3DController()override {}
		virtual ComponentType GetComponentType() const override { return m_type; };
		virtual IComponent* Copy(GameObject*const gameObject)const override;

		virtual bool Start()override;
		virtual bool Update() override;
		virtual bool Destroy() override;

		static ComponentType m_type;
	private:
	};

}