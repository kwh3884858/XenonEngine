#pragma once
#include "CrossPlatform/XenonManager.h"
#include "Algorithms/String.h"
//#include "CrossPlatform/Polygon/Polygon3D.h"
//#include "CrossPlatform/Material/Material.h"
//#include "CrossPlatform/Polygon/Polygon3D.h"
//namespace CrossPlatform
//{
//	class Polygon3D;
//}
namespace CrossPlatform
{
	class Mesh3DMeta;
}
namespace XenonEngine
{
	class Mesh3D;
	class ObjectImporter : public CrossPlatform::XenonManager<ObjectImporter>
	{
	public:
		virtual bool Initialize() override { return true; }
        virtual bool Shutdown() override { return true; }

		bool ImportObj(const Algorithm::String& path) const;
	private:

	};
}