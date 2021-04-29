#pragma once

#pragma comment(lib, "dsound.lib")
#include<DSOUND.h>

namespace WindowSound {
	class DirectXSoundConfig {
		HWND m_hwnd;
	};

	class DirectXSound
	{
	public:
		DirectXSound();
		~DirectXSound();

		void SetConfig(const DirectXSoundConfig* const config);
		void Initialize();
		void Update();
		void ShutDown();
	private:
		HWND m_hwnd;
		LPDIRECTSOUND lpds;


	};


}