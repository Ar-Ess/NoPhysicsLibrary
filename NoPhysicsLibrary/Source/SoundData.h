#pragma once

#include "Point.h"
#include "AcusticData.h"

struct SoundData
{
public:

	SoundData(int index, float pan, float volume) : index(index), volume(volume), pan(pan) {}

	int index = -1;
	float pan = 0;
	float volume = 1;
};
