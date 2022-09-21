#pragma once

#include "Point.h"

struct AcousticData
{
public:

	AcousticData(int index, Point position, float spl) : index(index), position(position), spl(spl) {}

	int index = -1;
	Point position = {};
	float spl = 0;
};
