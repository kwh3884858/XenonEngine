//  Graphic3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "CrossPlatform/XenonManager.h"
#include "Engine/Component/Transform3D.h"
#include "Algorithms/Vector.h"

namespace CrossPlatform 
{
	class Polygon3D;
}
namespace XenonEngine
{
	class Graphic3D :public CrossPlatform::XenonManager<Graphic3D>
	{
	public:
		virtual bool Initialize() override { return true; }
		virtual bool Shutdown() override { return true; }

		void AddGameobjectToRenderList(const GameObject* gameobject);
		void RemoveGameobjectFromRenderList(const GameObject* gameobject);
		void Render()const;
	private:
		Algorithm::Vector<GameObject*> m_renderList;
	};

}