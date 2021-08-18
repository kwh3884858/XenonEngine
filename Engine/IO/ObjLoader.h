#pragma once
#include "CrossPlatform/XenonManager.h"
#include "CrossPlatform/Polygon3D.h"
#include "Algorithms/String.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "Library/tiny_obj_loader.h"

namespace XenonEngine
{
	class ObjectLoader : public CrossPlatform::XenonManager<ObjectLoader>
	{
	public:
		virtual bool Initialize() override { return true; }
		virtual bool Shutdown() override;

		CrossPlatform::Polygon3D LoadObj(Algorithm::String& fileName );
	private:

	};
}