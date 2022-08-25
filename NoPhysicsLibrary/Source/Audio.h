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

	void LoadAudioSound(const char* filePath);

	void PlayAudio();

	void PlayAudioSound(float distance, bool shiftVolume, bool shiftBoth);

	void UnloadAudio();

	void UnloadAudioSound();

	ma_decoder decoder;
	ma_device device;

	ma_engine engine;
	ma_sound sound;

};
