#pragma once

#include "Point.h"

struct AcousticData
{
public:

	AcousticData(int index, Point emissionPosition, float spl) : index(index), emissionPosition(emissionPosition), spl(spl) {}

	int index = -1;
	Point emissionPosition = {};
	float spl = 0;
};
