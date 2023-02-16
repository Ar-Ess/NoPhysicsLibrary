#pragma once

struct SoundData
{
private:

	SoundData(unsigned int index, float pan, float volume, float delayTime) : index(index), volume(volume), pan(pan), delayTime(delayTime) {}

public:

	int index = -1;
	float pan = 0;
	float volume = 1;
	float delayTime = 0;

private:

	friend class NPL;
};
