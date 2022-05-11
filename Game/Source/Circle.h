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

	void SetPos(int x, int y)
	{
		this->x = (float)x;
		this->y = (float)y;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

	void SetCircle(Point position, float radius)
	{
		this->x = position.x;
		this->y = position.y;
		this->radius = radius;
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
