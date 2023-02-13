#pragma once

#include "PhysVec.h"

struct PhysRay
{
	PhysRay(PhysVec start, PhysVec end) : start(start), end(end) {}

	PhysVec start = {}, end = {};
};
