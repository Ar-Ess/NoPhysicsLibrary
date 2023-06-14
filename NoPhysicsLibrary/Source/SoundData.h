#pragma once

struct SoundData
{
	friend class NPL;
	friend class Acoustics;

private:

	SoundData(unsigned int index, float pan, float volume, float delayTime, float frequency, float resonance, float pitch) : 
		index(index), 
		volume(volume), 
		pan(pan), 
		delayTime(delayTime),
		frequency(frequency),
		resonance(resonance),
		pitch(pitch)
	{}

public:

	int index = -1;
	float volume = 1;
	float pan = 0;
	float delayTime = 0;
	float frequency = 0;
	float resonance = 0;
	float pitch = 1;

};
