#include "DirectXSound.h"

#include "cassert"
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
		assert(result == DS_OK);

        result = lpds->SetCooperativeLevel(m_hwnd, DSSCL_NORMAL);
        assert(result == DS_OK);

        memset(&pcmwf, 0, sizeof(pcmwf));
        pcmwf.wFormatTag = WAVE_FORMAT_PCM;
        pcmwf.nChannels = 1;
        pcmwf.nSamplesPerSec = 11025; //sample rate 11khz
        pcmwf.nBlockAlign = 1; 
        pcmwf.nAvgBytesPerSec = pcmwf.nSamplesPerSec*pcmwf.nBlockAlign;
        pcmwf.wBitsPerSample = 8;
        pcmwf.cbSize = 0;

        memset(&dsbd, 0, sizeof(DSBUFFERDESC));
        dsbd.dwSize = sizeof(DSBUFFERDESC);
        dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
        dsbd.dwBufferBytes = 22050; // 2 seconds 
        dsbd.lpwfxFormat = &pcmwf;
        result = lpds->CreateSoundBuffer(&dsbd, &lpdsBuffer, nullptr);

	}
	void DirectXSound::ShutDown()
	{
		if (lpds)
		{
			lpds->Release();
		}
	}
}