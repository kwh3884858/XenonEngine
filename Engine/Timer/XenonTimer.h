#pragma once
#include "CrossPlatform/Interface/ITimer.h"
#include "CrossPlatform/XenonManager.h"

namespace XenonEngine {
	class XenonTimer : public CrossPlatform::XenonManager<XenonTimer>
	{
	public:
		virtual bool Initialize() override { return true; }
		virtual bool Shutdown() override;

		void SetFileReader(CrossPlatform::ITimer*const timer);
		double GetTime()const;
	private:
		CrossPlatform::ITimer* m_timer;
	};
}