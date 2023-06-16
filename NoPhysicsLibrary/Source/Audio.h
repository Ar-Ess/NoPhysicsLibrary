#pragma once

#include "External/SoLoud/include/soloud.h"

#include "Sound.h"
#include "PhysArray.h"

struct SoundData;
class SoLoud::Soloud;
class SoLoud::BiquadResonantFilterInstance;
class PitchShiftFilter;

class Audio
{
public:

	~Audio();

	void Playback(SoundData* data);

	void LoadSound(const char* path);

	void CleanUp();

	int SoundSize() const
	{
		return sounds.Size();
	}

private:

	Audio();

private:

	friend class NoPhysicsLibrary;

	SoLoud::Soloud* audio = nullptr;
	SoLoud::BiquadResonantFilter* lowpass = nullptr;
	PitchShiftFilter* pitch = nullptr;
	PhysArray<Sound*> sounds;

};
