#pragma once
#include "CrossPlatform/XenonManager.h"
#include "Algorithms/String.h"

#define TINYOBJLOADER_IMPLEMENTATION 
#include "Library/tiny_obj_loader.h"
namespace CrossPlatform
{
	class Polygon3D;
}
namespace XenonEngine
{
	class ObjectLoader : public CrossPlatform::XenonManager<ObjectLoader>
	{
	public:
		virtual bool Initialize() override { return true; }
		virtual bool Shutdown() override;

		const CrossPlatform::Polygon3D* LoadObj(Algorithm::String& fileName );
	private:

	};
}