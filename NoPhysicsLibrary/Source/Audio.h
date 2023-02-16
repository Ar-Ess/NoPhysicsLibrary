#pragma once

#include "External/miniaudio/miniaudiodev.h" // Always first miniaudiodev
#include "External\dr_libs\dr_mp3.h" // Always second dr_mp3.h
#include "Sound.h"
#include "SoundData.h"
#include "SoundLoad.h"
#include "Define.h"
#include "PhysArray.h"

class Audio
{
public:

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

	Audio();

private:

	friend class NPL;

	ma_engine engine;
	PhysArray<SoundLoad*> sounds;
	PhysArray<Sound*> playback;

};
