#pragma once

#include "PhysVec.h"

struct AcousticData
{
public:

	AcousticData(int index, PhysVec emissionPosition, float spl) : index(index), emissionPosition(emissionPosition), spl(spl) {}

	int index = -1;
	PhysVec emissionPosition = {};
	float spl = 0;
};
