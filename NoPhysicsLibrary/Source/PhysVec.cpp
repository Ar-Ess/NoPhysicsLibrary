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

PhysVec::PhysVec(int x, float y)
{
	this->x = x;
	this->y = y;
	this->module = PhysMath::Module(x, y);
}

PhysVec::PhysVec(float x, int y)
{
	this->x = x;
	this->y = y;
	this->module = PhysMath::Module(x, y);
}

PhysVec PhysVec::operator*(PhysVec value) const
{
	return PhysVec(x * value.x, y * value.y);
}

PhysVec PhysVec::operator*(float value) const
{
	return PhysVec(x * value, y * value);
}

PhysVec PhysVec::operator*(int value) const
{
	return PhysVec(x * value, y * value);
}

PhysVec PhysVec::operator+(PhysVec value) const
{
	return PhysVec(x + value.x, y + value.y);
}

PhysVec PhysVec::operator+(float value) const
{
	return PhysVec(x + value, y + value);
}

PhysVec PhysVec::operator+(int value) const
{
	return PhysVec(x + value, y + value);
}

PhysVec PhysVec::operator-(PhysVec value) const
{
	return PhysVec(x - value.x, y - value.y);
}

PhysVec PhysVec::operator-(float value) const
{
	return PhysVec(x - value, y - value);
}

PhysVec PhysVec::operator-(int value) const
{
	return PhysVec(x - value, y - value);
}

PhysVec PhysVec::operator/(PhysVec value) const
{
	return PhysVec(x / value.x, y / value.y);
}

PhysVec PhysVec::operator/(float value) const
{
	return PhysVec(x / value, y / value);
}

PhysVec PhysVec::operator/(int value) const
{
	return PhysVec(x / value, y / value);
}

void PhysVec::operator=(float value)
{
	this->x = value;
	this->y = value;
}

void PhysVec::operator=(int value)
{
	this->x = value;
	this->y = value;
}

void PhysVec::operator*=(PhysVec value)
{
	this->x *= value.x;
	this->y *= value.y;
}

void PhysVec::operator*=(float value)
{
	this->x *= value;
	this->y *= value;
}

void PhysVec::operator*=(int value)
{
	this->x *= value;
	this->y *= value;
}

void PhysVec::operator+=(PhysVec value)
{
	this->x += value.x;
	this->y += value.y;
}

void PhysVec::operator+=(float value)
{
	this->x += value;
	this->y += value;
}

void PhysVec::operator+=(int value)
{
	this->x += value;
	this->y += value;
}

void PhysVec::operator-=(PhysVec value)
{
	this->x -= value.x;
	this->y -= value.y;
}

void PhysVec::operator-=(float value)
{
	this->x -= value;
	this->y -= value;
}

void PhysVec::operator-=(int value)
{
	this->x -= value;
	this->y -= value;
}

void PhysVec::Clear()
{
	x = 0;
	y = 0;
	module = 0.0f;
}

const bool PhysVec::IsZero() const
{
	return module == 0;
}
