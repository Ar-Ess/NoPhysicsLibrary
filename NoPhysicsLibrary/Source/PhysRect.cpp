#include "PhysRect.h"

PhysVec PhysRect::Position() const
{
	return PhysVec(x, y);
}

void PhysRect::Position(float x, float y)
{
	this->x = x;
	this->y = y;
}

void PhysRect::Position(PhysVec position)
{
	this->x = position.x;
	this->y = position.y;
}

PhysVec PhysRect::Size() const
{
	return PhysVec(w, h);
}

void PhysRect::Size(float w, float h)
{
	this->w = w;
	this->h = h;
}

void PhysRect::Size(PhysVec size)
{
	this->w = size.x;
	this->h = size.y;
}
