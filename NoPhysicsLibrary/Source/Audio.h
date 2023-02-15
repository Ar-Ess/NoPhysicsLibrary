#pragma once

#include "External\dr_libs\dr_mp3.h"
#include "Timer.h"
#include "Sound.h"
#include "SoundData.h"
#include "SoundLoad.h"
#include "Define.h"
#include "PhysArray.h"
#include "PhysVec.h"

class Audio
{
public:

	Audio();

	~Audio();

	void Playback(SoundData* data, float* dt);

	void Update();

	void LoadSound(const char* path);

	void CleanUp();

	const unsigned int SoundSize() const
	{
		return sounds.Size();
	}

private:

	ma_engine engine;
	PhysArray<SoundLoad*> sounds;
	PhysArray<Sound*> playback;

};
