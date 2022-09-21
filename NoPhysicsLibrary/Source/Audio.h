#pragma once

#include "External\dr_libs\dr_mp3.h"
#include "Sound.h"
#include "SoundData.h"
#include "Define.h"
#include <vector>
#include "Point.h"

class Audio
{
public:

	Audio();

	~Audio();

	void Playback(SoundData* data);

	void Update();

	void LoadSound(const char* path);

	void CleanUp();

private:

	ma_engine engine;
	std::vector<Sound*> sounds;
	std::vector<ma_sound*> playback;

};
