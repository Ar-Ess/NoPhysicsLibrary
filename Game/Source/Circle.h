#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "Point.h"

struct CircleCollider
{
public:

	CircleCollider() {}
	CircleCollider(float x_, float y_, float radius_) : x(x_), y(y_), radius(radius_) {}
	float x = 0.0f, y = 0.0f;
	float radius = 0.0f;

	void SetPos(int x_, int y_)
	{
		x = x_;
		y = y_;
	}

	void SetRadius(int radius_)
	{
		radius = radius_;
	}

	void SetCircle(int x_, int y_, int radius_)
	{
		x = x_;
		y = y_;
		radius = radius_;
	}

	float GetDiameter() const
	{
		return (radius * 2.0f);
	}

	Point GetAltLeftPosition() const
	{
		return { x - radius, y - radius };
	}
};

#endif
