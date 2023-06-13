#pragma once

struct SoundData
{
	friend class NPL;
	friend class Acoustics;

private:

	SoundData(unsigned int index, float pan, float volume, float delayTime, float frequency, float resonance) : 
		index(index), 
		volume(volume), 
		pan(pan), 
		delayTime(delayTime),
		frequency(frequency),
		resonance(resonance)
	{}

public:

	int index = -1;
	float pan = 0;
	float volume = 1;
	float delayTime = 0;
	float frequency = 0;
	float resonance = 0;

};
