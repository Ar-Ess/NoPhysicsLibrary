#pragma once

#include "Point.h"

struct SoundData
{
public:

	SoundData(int index, Point position) : index(index), position(position) {}

	int index = -1;
	Point position = {};
};
