#pragma once
#include "CrossPlatform/XenonManager.h"
#include "Algorithms/String.h"
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
        virtual bool Shutdown() override { return true; }

		const CrossPlatform::Polygon3D* LoadObj(Algorithm::String& fileName );
	private:

	};
}