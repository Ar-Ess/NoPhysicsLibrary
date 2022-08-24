#pragma once

#include "External\dr_libs\dr_mp3.h"
#include "External\miniaudio\miniaudio.h"

#include "Log.h"

class Audio
{
public:

	Audio();

	~Audio();

	void LoadAudio(const char* filePath);

	void PlayAudio();

	void UnloadAudio();

	ma_device device;
	ma_decoder decoder;
	ma_device_config deviceConfig;

};
