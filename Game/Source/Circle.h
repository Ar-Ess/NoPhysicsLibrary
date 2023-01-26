#pragma once

#include "Point.h"

struct Circle
{
public:

	Circle() {}
	Circle(float x, float y, float radius) : x(x), y(y), radius(radius) {}
	float x = 0.0f, y = 0.0f;
	float radius = 0.0f;

	float GetDiameter() const
	{
		return (radius * 2.0f);
	}

	Point GetAltLeftPosition() const
	{
		return { x - radius, y - radius };
	}
};
