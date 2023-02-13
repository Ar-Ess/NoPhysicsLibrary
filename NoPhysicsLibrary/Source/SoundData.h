#pragma once

struct SoundData
{
public:

	SoundData(int index, float pan, float volume, float delayTime) : index(index), volume(volume), pan(pan), delayTime(delayTime) {}

	int index = -1;
	float pan = 0;
	float volume = 1;
	float delayTime = 0;
};
