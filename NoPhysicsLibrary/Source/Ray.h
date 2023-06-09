#pragma once

#include "PhysVec.h"

struct PhysRay
{
	PhysRay() {}
	PhysRay(PhysVec start, PhysVec end) : start(start), end(end) {}

	PhysVec start = {}, end = {};

	float Distance()
	{
		if (start.IsZero() && end.IsZero()) return 0;
		float ret = end.module - start.module;

		return ret < 0 ? -ret : ret;
	}
};
