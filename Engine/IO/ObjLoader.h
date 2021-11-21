#pragma once
#include "CrossPlatform/XenonManager.h"
#include "Algorithms/String.h"
#include "CrossPlatform/Polygon/Polygon3D.h"
#include "CrossPlatform/Material/Material.h"
//#include "CrossPlatform/Polygon/Polygon3D.h"
//namespace CrossPlatform
//{
//	class Polygon3D;
//}
namespace XenonEngine
{
	class ObjectLoader : public CrossPlatform::XenonManager<ObjectLoader>
	{
	public:
		virtual bool Initialize() override { return true; }
        virtual bool Shutdown() override { return true; }

		bool LoadObj(const Algorithm::String& fileName, Algorithm::Vector<CrossPlatform::Polygon3D*>& polygons, Algorithm::Vector<CrossPlatform::Material*>& materials ) const;
	private:

	};
}