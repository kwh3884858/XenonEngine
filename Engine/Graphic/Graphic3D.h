//  Graphic3D.h
//  XenonEngine
//
//  Created by whkong on 2021-8-31.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "CrossPlatform/XenonManager.h"

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

		void RenderPolygon3D(const CrossPlatform::Polygon3D& polygon)const;
	private:

	};

}