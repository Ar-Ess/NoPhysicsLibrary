#pragma once

#include "PhysVec.h"

struct AcousticData
{
private:

	AcousticData(int index, float spl) : index(index), spl(spl) {}

public:

	int index = -1;
	float spl = 0;

private:

	friend class NPL;
	friend class Body;
};
