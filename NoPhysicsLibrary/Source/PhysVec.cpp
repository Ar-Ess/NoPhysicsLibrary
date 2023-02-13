#include "PhysMath.h"

PhysVec::PhysVec()
{
	Clear();
}

PhysVec::PhysVec(float x, float y)
{
	this->x = x;
	this->y = y;
	this->module = PhysMath::Module(x, y);
}

PhysVec::PhysVec(int x, int y)
{
	this->x = x;
	this->y = y;
	this->module = PhysMath::Module(x, y);
}

PhysVec PhysVec::operator*(PhysVec value)
{
	return PhysVec(x * value.x, y * value.y);
}

PhysVec PhysVec::operator*(float value)
{
	return PhysVec(x * value, y * value);
}

PhysVec PhysVec::operator*(int value)
{
	return PhysVec(x * value, y * value);
}

PhysVec PhysVec::operator+(PhysVec value)
{
	return PhysVec(x + value.x, y + value.y);
}

PhysVec PhysVec::operator+(float value)
{
	return PhysVec(x + value, y + value);
}

PhysVec PhysVec::operator+(int value)
{
	return PhysVec(x + value, y + value);
}

void PhysVec::Clear()
{
	x = 0;
	y = 0;
	module = 0.0f;
}
