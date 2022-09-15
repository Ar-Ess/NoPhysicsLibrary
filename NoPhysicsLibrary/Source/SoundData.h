#pragma once

#include "Point.h"

struct SoundData
{
public:

	SoundData(int index, Point position) : index(index), position(position) {}

	//-TODO: Canviar nom que no es el que toca
	void Set(float pan, float volume)
	{
		this->pan = pan;
		this->volume = volume;
	}

	int index = -1;
	Point position = {};
	float pan = 0;
	float volume = 1;
};
