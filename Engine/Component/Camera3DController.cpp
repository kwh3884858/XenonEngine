#include "Camera3DController.h"
#include "Transform3D.h"
#include "Engine/IO/InputSystem.h"
#include "CrossPlatform/XenonKey.h"
#include "MathLab/Vector.h"
#include "Engine/GameObject.h"

using namespace CrossPlatform;
using namespace MathLab;

namespace XenonEngine
{

	XenonEngine::IComponent* Camera3DController::Copy(GameObject*const gameObject) const
	{
		Camera3DController* cameraController = new Camera3DController(gameObject);
		return cameraController;
	}

	bool Camera3DController::Start()
	{
		return true;
	}

	bool Camera3DController::Update()
	{
		Transform3D* trans = GetGameObject()->GetComponentPointer<Transform3D>();
		float xAxisDelta = 0;
		float yAxisDelta = 0;
		float zAxisDelta = 0;
		static float velocity = 0.1f;

		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_W))
		{
			xAxisDelta = 1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_S))
		{
			xAxisDelta = -1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_Q))
		{
			yAxisDelta = 1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_E))
		{
			yAxisDelta = -1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_A))
		{
			zAxisDelta = 1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_D))
		{
			zAxisDelta = -1;
		}
		Vector3f delatPos(xAxisDelta * velocity, yAxisDelta*velocity, zAxisDelta* velocity);
		TVector3f tDeltaPos(delatPos);
		tDeltaPos = tDeltaPos * trans->GetRotationTranformMatrix();
		trans->AddPosition(tDeltaPos.GetVetor());

		float headingDelta = 0;
		float ElevationDelta = 0;
		if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_LEFT))
		{
			headingDelta = -1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_RIGHT))
		{
			headingDelta = 1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_UP))
		{
			ElevationDelta = 1;
		}
		else if (InputSystem::Get().GetKeyDown(CrossPlatform::XenonKey_DOWN))
		{
			ElevationDelta = -1;
		}
		trans->AddRotation(Vector3f(ElevationDelta, headingDelta, 0));
		return true;
	}

	bool Camera3DController::Destroy()
	{
		return true;

	}

	XenonEngine::ComponentType Camera3DController::m_type = ComponentType::ComponentType_CameraController;

}