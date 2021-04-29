#include "DirectXSound.h"
namespace WindowSound {
	DirectXSound::DirectXSound()
	{
	}

	DirectXSound::~DirectXSound()
	{
	}
	void DirectXSound::SetConfig(const DirectXSoundConfig* const config)
	{
		m_hwnd = config->m_hwnd;
	}
	void DirectXSound::Initialize()
	{
		HRESULT result;
		result = DirectSoundCreate(nullptr, &lpds, nullptr);
		assert(result == DI_OK);


	}
	void DirectXSound::ShutDown()
	{
		if (lpds)
		{
			lpds->Release();
		}
	}
}