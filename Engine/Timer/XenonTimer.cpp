#include "XenonTimer.h"
namespace XenonEngine
{
	bool XenonTimer::Shutdown()
	{
		delete m_timer;
		return true;
	}

	void XenonTimer::SetFileReader(CrossPlatform::ITimer * const timer)
	{
		m_timer = timer;
	}

	double XenonTimer::GetTime() const
	{
		return m_timer->GetTimeMilliSecond();
	}
}