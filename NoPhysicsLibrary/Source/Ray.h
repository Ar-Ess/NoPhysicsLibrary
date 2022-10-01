#pragma once

#include "Point.h"

struct Ray
{
public:

	Ray(Point origin, Point end) : origin(origin), end(end) {}

public:

	Point origin = {}, end = {};
};
