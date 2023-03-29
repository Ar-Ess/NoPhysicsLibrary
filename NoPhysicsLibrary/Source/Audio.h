#pragma once

#include "External/SoLoud/include/soloud.h"
#include "Sound.h"
#include "PhysArray.h"

class SoundData;

class Audio
{
public:

	~Audio();

	void Playback(SoundData* data, float* dt);

	void LoadSound(const char* path);

	void CleanUp();

	int SoundSize() const
	{
		return sounds.Size();
	}

private:

	Audio();

private:

	friend class NPL;

	SoLoud::Soloud* audio = nullptr;
	PhysArray<Sound*> sounds;

};
