#pragma once

#include "PhysVec.h"

struct AcousticData
{
private:

	AcousticData(int index, PhysVec emissionPosition, float spl) : index(index), emissionPosition(emissionPosition), spl(spl) {}

public:

	int index = -1;
	PhysVec emissionPosition = {};
	float spl = 0;

private:

	friend class NPL;
	friend class Body;
};
