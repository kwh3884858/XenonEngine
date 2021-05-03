//  DirectXSound.h
//  XenonEngine
//
//  Created by whkong on 2021-5-2.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

#pragma comment(lib, "dsound.lib")
#include<DSOUND.h>

namespace WindowSound {
	class DirectXSoundConfig {
    public:
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
        LPDIRECTSOUNDBUFFER lpdsBuffer;
        DSBUFFERDESC dsbd;
        WAVEFORMATEX pcmwf;
	};
}