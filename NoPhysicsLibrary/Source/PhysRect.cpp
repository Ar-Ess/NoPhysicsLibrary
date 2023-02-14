#include "PhysRect.h"

void PhysRect::operator*=(float value)
{
	this->x *= value;
	this->y *= value;
	this->w *= value;
	this->h *= value;
}

void PhysRect::operator*=(int value)
{
	this->x *= value;
	this->y *= value;
	this->w *= value;
	this->h *= value;
}

PhysRect PhysRect::operator*(float value) const
{
	return PhysRect(this->x * value,
		this->y * value,
		this->w * value,
		this->h * value);
}

PhysRect PhysRect::operator*(int value) const
{
	return PhysRect(this->x * value,
		this->y * value,
		this->w * value,
		this->h * value);
}

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

float PhysRect::Area() const
{
	return w * h;
}
