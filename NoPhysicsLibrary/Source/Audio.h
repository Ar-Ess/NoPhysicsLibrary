#pragma once

#include "External\dr_libs\dr_mp3.h"
#include "Timer.h"
#include "Sound.h"
#include "SoundData.h"
#include "SoundLoad.h"
#include "Define.h"
#include <vector>
#include "Point.h"

class Audio
{
public:

	Audio();

	~Audio();

	void Playback(SoundData* data, float* dt);

	void Update();

	void LoadSound(const char* path);

	void CleanUp();

	const size_t GetSoundSize() const
	{
		return sounds.size();
	}

private:

	ma_engine engine;
	std::vector<SoundLoad*> sounds;
	std::vector<Sound*> playback;

};
